use crate::ntclient::NtClient;
use crate::ntserver::NtServer;
use std::{io::prelude::*, net::TcpStream, sync::mpsc::Receiver, thread};

pub fn launch(serv_recv: Receiver<NtServer>, clnt_recv: Receiver<NtClient>) {
    thread::spawn(|| do_work(serv_recv, clnt_recv));
}

fn do_work(serv_recv: Receiver<NtServer>, clnt_recv: Receiver<NtClient>) {
    let mut servers: Vec<NtServer> = Vec::new();
    let mut newclients: Vec<NtClient> = Vec::new();
    let mut cnt = 0;
    loop {
        cnt += 1;
        if cnt >= 100 {
            cnt = 0;
            if servers.len() > 0 && servers[0].clients.len() > 0 {
                let corr =
                    b"This is dummy GNSS correction to test NTRIP client modem implementation. ";
                servers[0].clients[0].tcpstream.write_all(corr).unwrap();
            }
        }
        thread::sleep(std::time::Duration::from_millis(10));

        if let Ok(srv) = serv_recv.try_recv() {
            servers.push(srv);
        }

        if let Ok(cli) = clnt_recv.try_recv() {
            newclients.push(cli);
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
                        send_mounts(&cli.tcpstream);
                        newclients.swap_remove(pos);
                        eprintln!("{pos}: sent mount points");
                        continue;
                    } else if buf.starts_with(get_corr) {
                        send_icyok(&cli.tcpstream);
                        let ack_cli = newclients.swap_remove(pos);
                        if servers.len() > 0 {
                            servers[0].clients.push(ack_cli);
                            eprintln!("{pos}: NTRIP client accepted");
                        } else {
                            ack_cli
                                .tcpstream
                                .shutdown(std::net::Shutdown::Both)
                                .unwrap();
                            eprintln!("{pos}: no server available, client not accepted");
                        }
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

fn send_mounts(tcpstream: &TcpStream) {
    let mount_point = "STR;DynVrs;8021;CMR+;0(1),5(1);2;GPS+GLONASS;Single Base;ua;49.30;35.77;0;0;Trimble AgGPS_542;;B;N;9600;;\r\nENDSOURCETABLE\r\n";
    let response = format!("SOURCETABLE 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate:  4/22/2023:15:18:31 GMT\r\nContent-Type: gnss/sourcetable\r\nContent-Length: {}\r\n\r\n{}",
        mount_point.len(),
        mount_point);
    tcpstream
        .try_clone()
        .unwrap()
        .write_all(response.as_bytes())
        .unwrap();
    tcpstream.shutdown(std::net::Shutdown::Both).unwrap();
}

fn send_icyok(tcpstream: &TcpStream) {
    let response = b"ICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate:  4/22/2023:14:49: 4 GMT\r\nContent-Type: gnss/data\r\n";
    tcpstream.try_clone().unwrap().write_all(response).unwrap();
}