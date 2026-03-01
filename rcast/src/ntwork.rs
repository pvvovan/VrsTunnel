use crate::{cfg, cfg::*, ntclient::NtClient, ntserver::*, wgs84};
use std::{
    collections::HashMap,
    io::{ErrorKind, prelude::*},
    net::TcpStream,
    sync::mpsc::Receiver,
    thread,
};

pub fn launch(
    serv_recv: Receiver<NewServer>,
    clnt_recv: Receiver<NtClient>,
    cfg: HashMap<String, ServerCfg>,
) {
    thread::spawn(|| do_work(serv_recv, clnt_recv, cfg));
}

fn do_work(
    serv_recv: Receiver<NewServer>,
    clnt_recv: Receiver<NtClient>,
    cfg: HashMap<String, ServerCfg>,
) {
    let mut servers: Vec<AckServer> = Vec::new();
    let mut newservers: Vec<NewServer> = Vec::new();
    let mut newclients: Vec<NtClient> = Vec::new();
    let mut ackclients: Vec<NtClient> = Vec::new();

    loop {
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

        accept_newclients(&mut newclients, &mut ackclients);
        subscribe_clients(&mut ackclients, &mut servers, &cfg);

        thread::sleep(std::time::Duration::from_millis(10));
    }
}

fn subscribe_clients(
    ackclients: &mut Vec<NtClient>,
    servers: &mut Vec<AckServer>,
    cfg: &HashMap<String, ServerCfg>,
) {
    if servers.len() == 0 {
        return;
    }

    let mut cli_pos = 0;
    for _ in 0..ackclients.len() {
        let cli = &mut ackclients[cli_pos];
        let mut buf = [0u8; 4096];
        if let Ok(avail) = cli.tcpstream.read(&mut buf) {
            if avail > 0 {
                if cli.noavail_cnt > CLIENT_TIMEOUT {
                    cli.tcpstream
                        .shutdown(std::net::Shutdown::Both)
                        .unwrap_or_default();
                    ackclients.swap_remove(cli_pos);
                    continue;
                }

                let cli_location = crate::nmea::gga(&buf[0..avail]);
                if cli_location.is_err() && avail < 4000 {
                    cli.noavail_cnt += 1;
                    cli_pos += 1;
                    continue;
                }

                if cli_location.is_err() {
                    cli.tcpstream
                        .shutdown(std::net::Shutdown::Both)
                        .unwrap_or_default();
                    ackclients.swap_remove(cli_pos);
                    continue;
                }

                let cli_location = cli_location.unwrap();
                let mut min_distance = servers[0].location.distance(&cli_location);
                let mut min_serv_pos = 0;
                for i in 1..servers.len() {
                    let distance = servers[i].location.distance(&cli_location);
                    if distance < min_distance {
                        min_distance = distance;
                        min_serv_pos = i;
                    }
                }
                servers[min_serv_pos]
                    .clients
                    .push(ackclients.swap_remove(cli_pos));
                println!("{}: client subscribed", cli_pos);
                continue;
            }
        } else {
            cli.noavail_cnt += 1;
        }

        if cli.noavail_cnt > CLIENT_TIMEOUT {
            cli.tcpstream
                .shutdown(std::net::Shutdown::Both)
                .unwrap_or_default();
            ackclients.swap_remove(cli_pos);
            continue;
        }

        cli_pos += 1;
    }
}

fn is_server_timeout(serv: &mut AckServer) -> bool {
    serv.nocorr_cnt += 1;
    if serv.nocorr_cnt > SERVER_TIMEOUT {
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
            let req_ending = b"Transfer-Encoding: chunked\r\n\r\n";
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

fn accept_newclients(newclients: &mut Vec<NtClient>, ackclients: &mut Vec<NtClient>) {
    let mut pos = 0;
    for _ in 0..newclients.len() {
        let cli = &mut newclients[pos];
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
                    if is_icyok {
                        ackclients.push(ack_cli);
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
        } else {
            cli.noavail_cnt += 1;
        }

        if cli.noavail_cnt > CLIENT_TIMEOUT {
            cli.tcpstream
                .shutdown(std::net::Shutdown::Both)
                .unwrap_or_default();
            newclients.swap_remove(pos);
            continue;
        }

        pos += 1;
    }
}

fn send_mounts(tcpstream: &TcpStream) {
    let mount_point = format!(
        "STR;{};{};CMR+;0(1),5(1);2;GPS+GLONASS;Multi Base;ua;55.55;33.33;0;0;Super GNSS 1_1;;B;N;9600;;\r\nENDSOURCETABLE\r\n",
        cfg::MOUNT,
        cfg::CLIENT_PORT
    );
    let response = format!(
        "SOURCETABLE 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/sourcetable\r\nContent-Length: {}\r\n\r\n{}",
        datetimenow(),
        mount_point.len(),
        mount_point
    );
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        tcpstream.write_all(response.as_bytes()).unwrap_or_default();
        tcpstream
            .shutdown(std::net::Shutdown::Both)
            .unwrap_or_default();
    }
}

fn send_icyok(tcpstream: &TcpStream) -> Result<(), std::io::Error> {
    let response = format!(
        "ICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/data\r\n",
        datetimenow()
    );
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        return tcpstream.write_all(response.as_bytes());
    }
    Err(std::io::Error::from(ErrorKind::ConnectionRefused))
}

fn post_icyok(tcpstream: &TcpStream) -> Result<(), std::io::Error> {
    let response = format!(
        "HTTP/1.1 200 OK\r\nICY 200 OK\r\nNtrip-Version: Ntrip/1.0\r\nServer: NTRIP Caster 1.0\r\nDate: {}\r\nContent-Type: gnss/data\r\n\r\n",
        datetimenow()
    );
    if let Ok(mut tcpstream) = tcpstream.try_clone() {
        return tcpstream.write_all(response.as_bytes());
    }
    Err(std::io::Error::from(ErrorKind::ConnectionRefused))
}

pub fn datetimenow() -> String {
    let systime = std::time::SystemTime::now()
        .duration_since(std::time::SystemTime::UNIX_EPOCH)
        .expect("SystemTime failed")
        .as_secs() as i64;

    let unixdate = time::Date::from_calendar_date(1970, time::Month::January, 1)
        .expect("UnitDate failed")
        .checked_add(time::Duration::new(systime, 0))
        .unwrap();

    let month = unixdate.month() as u8;
    let day = unixdate.day();
    let year = unixdate.year();
    let hour = (systime % (24 * 60 * 60)) / (60 * 60);
    let minute = (systime % (60 * 60)) / 60;
    let second = systime % 60;
    format!(
        "{: >2}/{: >2}/{: >4}:{: >2}:{: >2}:{: >2} GMT",
        month, day, year, hour, minute, second
    )
}

const SERVER_TIMEOUT: i32 = 1000;
const CLIENT_TIMEOUT: i32 = 6000;
