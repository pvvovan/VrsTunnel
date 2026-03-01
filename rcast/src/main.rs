use std::io::Write;
use std::sync::mpsc;

mod cfg;
mod nmea;
mod ntclient;
mod ntserver;
mod ntwork;
mod wgs84;

fn main() {
    println!("NTRIP multi caster started running:");
    println!(
        "Listening on port {} for ntrip client connections",
        cfg::CLIENT_PORT
    );
    println!(
        "Listening on port {} for ntrip server connections",
        cfg::SERVER_PORT
    );

    let (serv_send, serv_recv) = mpsc::channel();
    let (clnt_send, clnt_recv) = mpsc::channel();
    ntclient::launch(clnt_send);
    ntserver::launch(serv_send);
    ntwork::launch(serv_recv, clnt_recv);
    println!("Quit [y/N]?");
    std::io::stdout().flush().unwrap();
    loop {
        let mut ans = String::new();
        std::io::stdin().read_line(&mut ans).unwrap();
        let ans = ans.trim_end();
        if ans.to_lowercase() == "y" {
            break;
        }
    }
    let _ = cfg::load()["h1"].clients.len();
}
