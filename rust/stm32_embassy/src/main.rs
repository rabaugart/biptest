#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Input, Output, Pull, Speed};
use embassy_time::{Duration, Timer};
use {defmt_rtt as _, panic_probe as _}; // global logger

#[embassy_executor::task]
async fn blinker(mut led: Output<'static>, button: Input<'static>, interval: Duration) {
    loop {
        let ivb = if button.is_high() { interval } else { interval*2 };
        led.set_high();
        Timer::after(ivb).await;
        led.set_low();
        Timer::after(ivb).await;
    }
}

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let led = Output::new(p.PB3, Level::Low, Speed::Low);
    let button = Input::new(p.PB4,Pull::Down);
    let _ = spawner.spawn(blinker(led, button, Duration::from_millis(1_000)));
}
