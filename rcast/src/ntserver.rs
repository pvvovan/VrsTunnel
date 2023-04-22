use crate::ntclient::NtClient;
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(serv_sender: Sender<NtServer>) {
    thread::spawn(|| accept_servers(serv_sender));
}

fn accept_servers(serv_sender: Sender<NtServer>) {
    serv_sender.send(NtServer { clients: vec![] }).unwrap();
    loop {
        thread::sleep(std::time::Duration::from_secs(1));
        // eprintln!("server correction");
    }
}

pub struct NtServer {
    pub clients: Vec<NtClient>,
}
