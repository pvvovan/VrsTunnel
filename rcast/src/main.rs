use std::io::Write;
use std::sync::mpsc;

mod cfg;
mod nmea;
mod ntclient;
mod ntserver;
mod ntwork;
mod wgs84;

fn main() {
    if nmea::gga(b"031*4F\r\n$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0 0031*4F\r\n$GPGGA").is_err() {
        return;
    }

    println!("NTRIP multi caster started running...");
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
}
