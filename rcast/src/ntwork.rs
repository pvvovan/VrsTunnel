use crate::ntclient::NtClient;
use crate::ntserver::NtServer;
use std::{io::prelude::*, sync::mpsc::Receiver, thread};

pub fn launch(serv_recv: Receiver<NtServer>, clnt_recv: Receiver<NtClient>) {
    thread::spawn(|| do_work(serv_recv, clnt_recv));
}

fn do_work(serv_recv: Receiver<NtServer>, clnt_recv: Receiver<NtClient>) {
    let mut servers: Vec<NtServer> = Vec::new();
    let mut newclients: Vec<NtClient> = Vec::new();
    loop {
        thread::sleep(std::time::Duration::from_millis(10));

        if let Ok(srv) = serv_recv.try_recv() {
            servers.push(srv);
        }

        if let Ok(cli) = clnt_recv.try_recv() {
            newclients.push(cli);
            eprintln!("new client added");
        }

        let mut pos = 0;
        for _ in 0..newclients.len() {
            let cli = &newclients[pos];
            let mut buf = [0u8; 4096];
            let get_mounts = b"GET / ";
            let get_corr = b"GET /DynVrs";
            if let Ok(avail) = cli.tcpstream.peek(&mut buf) {
                if avail >= get_mounts.len() {
                    if buf.starts_with(get_mounts) {
                        let mount_point = "STR;DynVrs;DynVrs;RTCM 3;;2;GPS+GLO;;;51.52;30.75;1;1;Rust GNSS Correction;none;B;Y;9600;";
                        let response = format!(
                            "{}\r\nContent-Length: {}\r\n\r\n{}\r\nENDSOURCETABLE\r\n",
                            "HTTP/1.0 200 OK",
                            mount_point.len(),
                            mount_point
                        );
                        cli.tcpstream
                            .try_clone()
                            .unwrap()
                            .write_all(response.as_bytes())
                            .unwrap();
                        cli.tcpstream.shutdown(std::net::Shutdown::Both).unwrap();
                        newclients.swap_remove(pos);
                        eprintln!("{pos} disconnected");
                        continue;
                    } else if buf.starts_with(get_corr) {
                        let mount_point = "STR;CORR;CORR;RTCM 3;;2;GPS+GLO;;;51.52;30.75;1;1;Rust GNSS Correction;none;B;Y;9600;";
                        let response = format!(
                            "{}\r\nContent-Length: {}\r\n\r\n{}\r\nENDSOURCETABLE\r\n",
                            "HTTP/1.0 200 OK",
                            mount_point.len(),
                            mount_point
                        );
                        cli.tcpstream
                            .try_clone()
                            .unwrap()
                            .write_all(response.as_bytes())
                            .unwrap();
                        cli.tcpstream.shutdown(std::net::Shutdown::Both).unwrap();
                        newclients.swap_remove(pos);
                        eprintln!("{pos}: correction requested");
                        continue;
                    } else {
                        cli.tcpstream.shutdown(std::net::Shutdown::Both).unwrap();
                        newclients.swap_remove(pos);
                        eprintln!("{pos} no mount");
                        continue;
                    }
                }
            }
            pos += 1;
        }
    }
}
