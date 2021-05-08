use std::fmt;

pub struct Mess {
    pub temp: f32,
    pub feucht: f32,
    pub druck: f32,
}

fn p32(jv: &json::JsonValue, s: &String) -> f32 {
    match jv[s].as_f32() {
        Some(v) => v,
        None => {
            error!("Error parsing field {}", s);
            std::f32::NAN
        }
    }
}

pub fn from_json(jv: json::JsonValue) -> Mess {
    Mess {
        temp: p32(&jv, &String::from("t")),
        feucht: p32(&jv, &String::from("h")),
        druck: p32(&jv, &String::from("p")),
    }
}

impl fmt::Display for Mess {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "temp:{}, feucht:{}, druck:{}",
            self.temp, self.feucht, self.druck
        )
    }
}
