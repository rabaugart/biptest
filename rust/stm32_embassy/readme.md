# Rust auf stm32 mit embassy

31.8.2025

Flashen mit `probe-rs` über

    cargo run --release

Evtl. vorher `probe-rs` und das Target installieren

    cargo install probe-rs-tools
    rustup target add thumbv7em-none-eabi

Funktioniert auf Mac und Ubuntu 24.04 grundsätzlich. Es gibt aber eine Fehlermeldung

~~~~~~
     Running `probe-rs run --chip STM32L432KCUx --connect-under-reset --speed 3300 target/thumbv7em-none-eabi/release/rust_embassy`
     Erasing sectors ✔ [00:00:00] [] 20.00 KiB/20.00 KiB @ 41.64 KiB/s (eta 0s )
 Programming pages   ✔ [00:00:00] [] 19.00 KiB/19.00 KiB @ 23.63 KiB/s (eta 0s )    Finished in 1.311s
TRACE BDCR ok: 00008200
└─ <invalid location: defmt frame-index: 7>
DEBUG rcc: Clocks { hclk1: MaybeHertz(4000000), hclk2: MaybeHertz(4000000), hclk3: MaybeHertz(4000000), hse: MaybeHertz(0), hsi: MaybeHertz(0), hsi48: MaybeHertz(48000000), lse: MaybeHertz(0), lsi: MaybeHertz(32000), msi: MaybeHertz(4000000), pclk1: MaybeHertz(4000000), pclk1_tim: MaybeHertz(4000000), pclk2: MaybeHertz(4000000), pclk2_tim: MaybeHertz(4000000), pll1_p: MaybeHertz(0), pll1_q: MaybeHertz(0), pllsai1_p: MaybeHertz(0), pllsai1_q: MaybeHertz(0), pllsai2_p: MaybeHertz(0), rtc: MaybeHertz(32000), sai1_extclk: MaybeHertz(0), sys: MaybeHertz(4000000) }
└─ <invalid location: defmt frame-index: 9>
~~~~~~

Abbrechen mit `Ctrl-c`. Das Board ist aber geflasht.
