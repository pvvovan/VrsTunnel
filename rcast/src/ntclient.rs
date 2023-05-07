use crate::cfg;
use std::net::{TcpListener, TcpStream};
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(clnt_sender: Sender<NtClient>) {
    thread::spawn(|| accept_clients(clnt_sender));
}

fn accept_clients(clnt_sender: Sender<NtClient>) {
    let addr = format!("{}:{}", cfg::BIND_ADDR, cfg::CLIENT_PORT);
    let tcplistener = TcpListener::bind(addr).expect("Ntrip client bind failed");
    loop {
        let tcpconn = tcplistener.accept();
        if tcpconn.is_ok() {
            if let Ok((tcpstream, sockaddr)) = tcpconn {
                if tcpstream.set_nonblocking(true).is_ok() {
                    clnt_sender
                        .send(NtClient {
                            tcpstream,
                            noavail_cnt: 0,
                        })
                        .unwrap();
                    println!("client connected: {}", sockaddr.ip().to_string());
                }
            }
        }
    }
}

pub struct NtClient {
    pub tcpstream: TcpStream,
    pub noavail_cnt: i32,
}
