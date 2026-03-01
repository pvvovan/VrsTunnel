use std::io::Write;
use std::sync::mpsc;

mod cfg;
mod nmea;
mod ntclient;
mod ntserver;
mod ntwork;
mod wgs84;

fn main() {
    let args = std::env::args().collect::<Vec<String>>();
    if args.len() != 2 {
        println!("Path to config missing!");
        return;
    }
    if !std::fs::exists(&args[1]).expect(&format!("Failed to check {}!", &args[1])) {
        println!("File {} doesn't exist!", &args[1]);
        return;
    }
    let json_str =
        std::fs::read_to_string(&args[1]).expect(&format!("Failed to read {}!", &args[0]));

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
    ntwork::launch(serv_recv, clnt_recv, cfg::parse(&json_str));
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
}
