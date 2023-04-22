use crate::ntclient::NtClient;
use std::sync::mpsc::Sender;
use std::thread;

pub fn launch(serv_sender: Sender<NtServer>) {
    thread::spawn(|| accept_servers(serv_sender));
}

fn accept_servers(_serv_sender: Sender<NtServer>) {
    loop {
        thread::sleep(std::time::Duration::from_secs(1));
        // eprintln!("server correction");
    }
}

pub struct NtServer {
    pub clients: Vec<NtClient>,
}
