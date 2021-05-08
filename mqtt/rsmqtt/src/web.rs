use std::sync::mpsc;
use std::thread;

use crate::messdata::Mess;

pub struct Receiver {
    s: mpsc::Sender<Mess>,
}

pub fn receive(r: &Receiver, m: Mess) {
    r.s.send(m).unwrap();
}

pub fn start() -> Receiver {
    let (tx, rx) = mpsc::channel();
    thread::spawn(move || loop {
        info!("web {}", rx.recv().unwrap());
    });
    Receiver { s: tx }
}
