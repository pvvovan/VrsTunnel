use crate::{cfg, ntclient::NtClient, ntserver::*, wgs84};
use chrono::{self, Datelike, Timelike};
use std::{
    io::{prelude::*, ErrorKind},
    net::TcpStream,
    sync::mpsc::Receiver,
    thread,
};

pub fn launch(serv_recv: Receiver<NewServer>, clnt_recv: Receiver<NtClient>) {
    thread::spawn(|| do_work(serv_recv, clnt_recv));
}

fn do_work(serv_recv: Receiver<NewServer>, clnt_recv: Receiver<NtClient>) {
    let mut servers: Vec<AckServer> = Vec::new();

    loop {
        let mut newservers: Vec<NewServer> = Vec::new();
        let mut newclients: Vec<NtClient> = Vec::new();

        while let Ok(srv) = serv_recv.try_recv() {
            newservers.push(srv);
        }

        while let Ok(cli) = clnt_recv.try_recv() {
            newclients.push(cli);
        }

        accept_newservers(&mut newservers, &mut servers);

        let mut serv_pos = 0;
        for _ in 0..servers.len() {
            let serv = &mut servers[serv_pos];
            let mut buf = [0u8; 4096];
            if let Ok(avail) = serv.tcpstream.read(&mut buf) {
                if avail > 0 {
                    // println!("server avail: {avail}");
                    serv.nocorr_cnt = 0;
                    let corr = &buf[0..avail];
                    let mut cli_pos = 0;
                    for _ in 0..serv.clients.len() {
                        let cli = &mut serv.clients[cli_pos];
                        if cli.tcpstream.write(corr).is_err() {
                            cli.tcpstream
                                .shutdown(std::net::Shutdown::Both)
                                .unwrap_or_default();
                            serv.clients.swap_remove(cli_pos);
                            println!("client closed");
                            continue;
                        }
                        cli_pos += 1;
                    }
                } else {
                    if is_server_timeout(serv) {
                        servers.swap_remove(serv_pos);
                        continue;
                    }
                }
            } else {
                if is_server_timeout(serv) {
                    servers.swap_remove(serv_pos);
                    continue;
                }
            }
            serv_pos += 1;
        }

        accept_newclients(&mut newclients, &mut servers);

        thread::sleep(std::time::Duration::from_millis(10));
    }
}

fn is_server_timeout(serv: &mut AckServer) -> bool {
    serv.nocorr_cnt += 1;
    if serv.nocorr_cnt > 1000 {
        for cli in serv.clients.iter() {
            cli.tcpstream
                .shutdown(std::net::Shutdown::Both)
                .unwrap_or_default();
        }
        serv.tcpstream
            .shutdown(std::net::Shutdown::Both)
            .unwrap_or_default();
        println!("server closed");
        return true;
    }
    return false;
}

fn accept_newservers(newservers: &mut Vec<NewServer>, servers: &mut Vec<AckServer>) {
    let mut pos = 0;
    for _ in 0..newservers.len() {
        let serv = &newservers[pos];
        let mut buf = [0u8; 4096];
        if let Ok(avail) = serv.tcpstream.peek(&mut buf) {
            let post_req = b"POST /";
            let req_ending = b"Transfer-Encoding: chunked";
            if avail > post_req.len() + req_ending.len() {
                let new_serv = newservers.swap_remove(pos);
                let new_serv_tcpstream = new_serv.tcpstream.try_clone().unwrap();
                let buf = &buf[0..avail];

                let mut is_srv_ok = buf.starts_with(post_req);

                if is_srv_ok {
                    is_srv_ok = buf.ends_with(req_ending);
                }

                let mut req_str = String::new();
                if is_srv_ok {
                    if let Ok(st) = std::str::from_utf8(buf) {
                        req_str = st.to_string();
                    } else {
                        is_srv_ok = false;
                    }
                }

                let req_str: Vec<&str> = req_str.lines().collect();
                if is_srv_ok {
                    is_srv_ok = req_str.len() > 5;
                }

                let mut lat = 0.0;
                let mut lon = 0.0;
                if is_srv_ok {
                    let strs: Vec<&str> = req_str[5].split(";").collect();
                    is_srv_ok = strs.len() > 8;
                    if is_srv_ok {
                        match strs[7].parse() {
                            Ok(num) => lat = num,
                            _ => is_srv_ok = false,
                        }
                    }

                    if is_srv_ok {
                        match strs[8].parse() {
                            Ok(num) => lon = num,
                            _ => is_srv_ok = false,
                        }
                    }
                }

                if is_srv_ok {
                    is_srv_ok = post_icyok(&new_serv.tcpstream).is_ok();
                }

                if is_srv_ok {
                    let geo = wgs84::Location::new(lat, lon, 0.0);
                    let geo2 = wgs84::Location::new(lat, 0.0, 0.0);
                    println!("Distance: {}", geo.distance(&geo2));
                    servers.push(AckServer {
                        tcpstream: new_serv.tcpstream,
                        clients: vec![],
                        location: wgs84::Location::new(lat, lon, 0.0),
                        nocorr_cnt: 0,
                    });
                    eprintln!("{pos}: NTRIP server accepted");
                }

                if !is_srv_ok {
                    new_serv_tcpstream
                        .shutdown(std::net::Shutdown::Both)
                        .unwrap_or_default();
                }
                continue;
            }
        }
        pos += 1;
    }
}

fn accept_newclients(newclients: &mut Vec<NtClient>, servers: &mut Vec<AckServer>) {
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
                        servers[0].clients.push(ack_cli); // select server?
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

fn send_mounts(tcpstream: &TcpStream) {
    let mount_point = format!("STR;{};{};CMR+;0(1),5(1);2;GPS+GLONASS;Multi Base;ua;55.55;33.33;0;0;Super GNSS 1_1;;B;N;9600;;\r\nENDSOURCETABLE\r\n", cfg::MOUNT, cfg::CLIENT_PORT);
    let response = format!("SOURCETABLE 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/sourcetable\r\nContent-Length: {}\r\n\r\n{}",
        datetimenow(),
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
    let response = format!("ICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/data\r\n",
        datetimenow());
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        return tcpstream.write_all(response.as_bytes());
    }
    Err(std::io::Error::from(ErrorKind::ConnectionRefused))
}

fn post_icyok(tcpstream: &TcpStream) -> Result<(), std::io::Error> {
    let response = format!("HTTP/1.1 200 OK\r\nICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/data\r\n\r\n",
        datetimenow());
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        return tcpstream.write_all(response.as_bytes());
    }
    Err(std::io::Error::from(ErrorKind::ConnectionRefused))
}

fn datetimenow() -> String {
    let dt = chrono::Utc::now();
    let m = dt.month();
    let d = dt.day();
    let y = dt.year();
    let h = dt.hour();
    let mm = dt.minute();
    let s = dt.second();
    format!(
        "{: >2}/{: >2}/{: >4}:{: >2}:{: >2}:{: >2} GMT",
        m, d, y, h, mm, s
    )
}
