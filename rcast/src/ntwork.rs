use crate::{cfg, ntclient::NtClient, ntserver::NtServer};
use std::{
    io::{prelude::*, ErrorKind},
    net::TcpStream,
    sync::mpsc::Receiver,
    thread,
};

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
                if servers[0].clients[0].tcpstream.write_all(corr).is_err() {
                    servers[0].clients[0]
                        .tcpstream
                        .shutdown(std::net::Shutdown::Both)
                        .unwrap_or_default();
                    servers[0].clients.clear();
                }
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
            let get_corr = format!("GET /{}", cfg::MOUNT);
            if let Ok(avail) = cli.tcpstream.peek(&mut buf) {
                if avail >= get_mounts.len() {
                    if buf.starts_with(get_mounts) {
                        send_mounts(&cli.tcpstream);
                        newclients.swap_remove(pos);
                        eprintln!("{pos}: sent mount points");
                        continue;
                    } else if buf.starts_with(get_corr.as_bytes()) {
                        let is_icyok = send_icyok(&cli.tcpstream).is_ok();
                        let ack_cli = newclients.swap_remove(pos);
                        if servers.len() > 0 && is_icyok {
                            servers[0].clients.push(ack_cli);
                            eprintln!("{pos}: NTRIP client accepted");
                        } else {
                            ack_cli
                                .tcpstream
                                .shutdown(std::net::Shutdown::Both)
                                .unwrap_or_default();
                            eprintln!("{pos}: no server available, client not accepted");
                        }
                        continue;
                    } else {
                        cli.tcpstream
                            .shutdown(std::net::Shutdown::Both)
                            .unwrap_or_default();
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
    let mount_point = format!("STR;{};{};CMR+;0(1),5(1);2;GPS+GLONASS;Multi Base;ua;55.55;33.33;0;0;Super GNSS 1_1;;B;N;9600;;\r\nENDSOURCETABLE\r\n", cfg::MOUNT, cfg::CLIENT_PORT);
    let response = format!("SOURCETABLE 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate:  4/22/2023:15:18:31 GMT\r\nContent-Type: gnss/sourcetable\r\nContent-Length: {}\r\n\r\n{}",
        mount_point.len(),
        mount_point);
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        tcpstream.write_all(response.as_bytes()).unwrap_or_default();
        tcpstream
            .shutdown(std::net::Shutdown::Both)
            .unwrap_or_default();
    }
}

fn send_icyok(tcpstream: &TcpStream) -> Result<(), std::io::Error> {
    let response = b"ICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate:  4/22/2023:14:49: 4 GMT\r\nContent-Type: gnss/data\r\n";
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        return tcpstream.write_all(response);
    }
    Err(std::io::Error::from(ErrorKind::ConnectionRefused))
}
