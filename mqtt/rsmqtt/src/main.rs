#[macro_use]
extern crate log;
extern crate clap;
extern crate json;
extern crate uuid;

mod messdata;

mod web;

mod mqttcl;

fn main() {
    let recv = web::start();
    mqttcl::mqtt_start(|m| {
        web::receive(&recv, m);
    });
}
