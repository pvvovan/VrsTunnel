use crate::ntclient::NtClient;
use crate::{cfg, wgs84};
use std::net::{TcpListener, TcpStream};
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(serv_sender: Sender<NewServer>) {
    thread::spawn(|| accept_servers(serv_sender));
}

fn accept_servers(serv_sender: Sender<NewServer>) {
    let addr = format!("{}:{}", cfg::BIND_ADDR, cfg::SERVER_PORT);
    let tcplistener = TcpListener::bind(addr).expect("Ntrip server bind failed");
    loop {
        let tcpconn = tcplistener.accept();
        if tcpconn.is_ok() {
            if let Ok((tcpstream, sockaddr)) = tcpconn {
                if tcpstream.set_nonblocking(true).is_ok() {
                    serv_sender.send(NewServer { tcpstream }).unwrap();
                    println!("server connected: {}", sockaddr.ip().to_string());
                }
            }
        }
    }
}

pub struct NewServer {
    pub tcpstream: TcpStream,
}

pub struct AckServer {
    pub tcpstream: TcpStream,
    pub clients: Vec<NtClient>,
    pub nocorr_cnt: i32,
    pub location: wgs84::Location,
}
