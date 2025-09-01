#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::{
    Peripherals,
    exti::ExtiInput,
    gpio::{Level, Output, Pull, Speed},
};
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};
use embassy_time::{Duration, Instant, Timer};
use {defmt_rtt as _, panic_probe as _}; // global logger

const NUM_SEGS: usize = 7;

#[embassy_executor::task]
async fn button_task(
    mut e: ExtiInput<'static>,
    sender: Sender<'static, CriticalSectionRawMutex, Command, 5>,
) {
    let mut state: u32 = 1;
    loop {
        e.wait_for_rising_edge().await;
        let presstime = Instant::now();
        e.wait_for_falling_edge().await;
        let releasetime = Instant::now();
        if releasetime - presstime > Duration::from_millis(50)
            && releasetime - presstime < Duration::from_millis(2_000)
        {
            info!("Button gedrückt");
            sender.send(Command::NeueGeschwindigkeit(state + 1)).await;
            state = 1 - state;
        }
    }
}
#[embassy_executor::task]
async fn blinker(mut so: SegOutput<'static>, interval: Duration, ch: &'static Channel) {
    let recv = ch.receiver();
    let mut ivb = interval;
    loop {
        for zi in ZIFFERN.iter().cycle().take(15) {
            so.zeige(zi.1);
            Timer::after(ivb).await;
            if let Ok(c) = recv.try_receive() {
                match c {
                    Command::NeueGeschwindigkeit(s) => ivb = s * interval,
                }
            }
        }
        so.zeige(Bitmuster::Aus);
        info!("Waiting for command");
        let c = recv.receive().await;
        match c {
            Command::NeueGeschwindigkeit(s) => ivb = s * interval,
        }
    }
}

#[derive(Copy, Clone)]
enum Bitmuster {
    Ziffer(u8),
    Aus,
}

const ZIFFERN: &[(char, Bitmuster)] = &[
    //C      A  B  C  D  E  F  G
    ('0', zm(1, 1, 1, 1, 1, 1, 0)),
    ('1', zm(0, 1, 1, 0, 0, 0, 0)),
    ('2', zm(1, 1, 0, 1, 1, 0, 1)),
    ('3', zm(1, 1, 1, 1, 0, 0, 1)),
    ('4', zm(0, 1, 1, 0, 0, 1, 1)),
    ('5', zm(1, 0, 1, 1, 0, 1, 1)),
    ('6', zm(1, 0, 1, 1, 1, 1, 1)),
    ('7', zm(1, 1, 1, 0, 0, 0, 0)),
    ('8', zm(1, 1, 1, 1, 1, 1, 1)),
    ('9', zm(1, 1, 1, 1, 0, 1, 1)),
];

const fn zm(a: u8, b: u8, c: u8, d: u8, e: u8, f: u8, g: u8) -> Bitmuster {
    Bitmuster::Ziffer(a | (b << 1) | (c << 2) | (d << 3) | (e << 4) | (f << 5) | (g << 6))
}

struct SegOutput<'a> {
    out: [Output<'a>; NUM_SEGS + 1],
}

impl<'a> SegOutput<'a> {
    fn new(p: Peripherals) -> Self {
        Self {
            out: [
                Output::new(p.PB1, Level::Low, Speed::Low),  // Segment A
                Output::new(p.PA9, Level::Low, Speed::Low),  // Segment B
                Output::new(p.PA10, Level::Low, Speed::Low), // Segment C
                Output::new(p.PB0, Level::Low, Speed::Low),  // Segment D
                Output::new(p.PB5, Level::Low, Speed::Low),  // Segment E
                Output::new(p.PA1, Level::Low, Speed::Low),  // Segment F
                Output::new(p.PA0, Level::Low, Speed::Low),  // Segment G
                Output::new(p.PB3, Level::Low, Speed::Low),  // Segment DP
            ],
        }
    }

    fn zeige(&mut self, b: Bitmuster) {
        match b {
            Bitmuster::Ziffer(z) => {
                for i in 0..NUM_SEGS {
                    if z & (1 << i) == 0 {
                        self.out[i].set_high();
                    } else {
                        self.out[i].set_low();
                    }
                }
            }
            Bitmuster::Aus => self.zeige(Bitmuster::Ziffer(0)),
        }
    }
}

enum Command {
    NeueGeschwindigkeit(u32),
}

type Channel = embassy_sync::channel::Channel<CriticalSectionRawMutex, Command, 5>;

static CHA: Channel = Channel::new();

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    info!("Starte");
    unsafe {
        let button = ExtiInput::new(
            p.PB4.clone_unchecked(),
            p.EXTI4.clone_unchecked(),
            Pull::Down,
        );
        let _ = spawner.spawn(button_task(button, CHA.sender()));
    }
    let so = SegOutput::new(p);
    let _ = spawner.spawn(blinker(so, Duration::from_millis(1_000), &CHA));
}
