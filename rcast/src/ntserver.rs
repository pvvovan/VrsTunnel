use crate::cfg;
use crate::ntclient::NtClient;
use std::net::{TcpListener, TcpStream};
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(serv_sender: Sender<NtServer>) {
    thread::spawn(|| accept_servers(serv_sender));
}

fn accept_servers(serv_sender: Sender<NtServer>) {
    let addr = format!("{}:{}", cfg::BIND_ADDR, cfg::SERVER_PORT);
    let tcplistener = TcpListener::bind(addr).expect("Ntrip server bind failed");
    loop {
        let tcpconn = tcplistener.accept();
        if tcpconn.is_ok() {
            if let Ok((tcpstream, sockaddr)) = tcpconn {
                if tcpstream.set_nonblocking(true).is_ok() {
                    serv_sender
                        .send(NtServer {
                            tcpstream,
                            clients: vec![],
                            nocorr_cnt: 0,
                        })
                        .unwrap();
                    println!("server connected: {}", sockaddr.ip().to_string());
                }
            }
        }
    }
}

pub struct NtServer {
    pub tcpstream: TcpStream,
    pub clients: Vec<NtClient>,
    pub nocorr_cnt: i32,
}
