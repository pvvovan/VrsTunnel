use std::net::{TcpListener, TcpStream};
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(clnt_sender: Sender<NtClient>) {
    thread::spawn(|| accept_clients(clnt_sender));
}

fn accept_clients(clnt_sender: Sender<NtClient>) {
    let tcplistener = TcpListener::bind("127.0.0.1:8021").expect("Ntcip client bind failed");
    loop {
        let tcpconn = tcplistener.accept();
        if tcpconn.is_ok() {
            let (tcpstream, sockaddr) = tcpconn.unwrap();
            eprintln!("client connected: {}", sockaddr.ip().to_string());
            tcpstream.set_nonblocking(true).unwrap();
            clnt_sender.send(NtClient { tcpstream }).unwrap();
        }
    }
}

pub struct NtClient {
    pub tcpstream: TcpStream,
}
