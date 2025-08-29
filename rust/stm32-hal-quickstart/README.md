# Raba, 13.9.23

Blinky für das Nucleo-Board stm32l432kc

Geklont von https://github.com/David-OConnor/stm32-hal-quickstart.git

[Stand](https://github.com/David-OConnor/stm32-hal-quickstart/commit/a068fe210a3db365b4c168b6960f1a21c933168d)

```
rustup target add thumbv7em-none-eabihf

cargo install flip-link

cargo install probe-rs --features cli
```

Unter Linux wird für `probe-rs` das Paket `libudev-dev` benötigt.

Bauen und Flashen mit

```
cargo-flash --chip STM32L432KCUx
```

## Links

* [cargo-flash](https://probe.rs/docs/tools/cargo-flash/)

# STM32-HAL quickstart

This repo provides a starting point for new [STM32-HAL](https://github.com/David-OConnor/stm32-hal)
projects. It's based on the [Knurling app template](https://github.com/knurling-rs/app-template).

# Quickstart
- [Install Rust](https://www.rust-lang.org/tools/install).
- Install the compilation target for your MCU. Eg run `rustup target add thumbv7em-none-eabihf`. You'll need to change the last part if using a Cortex-M0, Cortex-M33, (Eg Stm32G0 or L5 respectively) or if you don't want to use hardware floats.
- Install flash and debug tools: `cargo install flip-link`, `cargo install probe-run`.
- Clone this repo: `git clone https://github.com/David-OConnor/stm32-hal-quickstart`
- Change the following lines to match your MCU. Post an issue if you need help with this:
  - `Cargo.toml`: `stm32-hal2 = { version = "^1.1.0", features = ["l4x3", "l4rt"]}`
  - `memory.x`: `FLASH` and `RAM` lines
  - `.cargo/config.toml`: `runner` and `target` lines.
- Connect your device. Run `cargo run --release` to compile and flash.
