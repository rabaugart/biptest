#![no_std]
#![no_main]
#![feature(abi_avr_interrupt)]

use core::cell::Cell;

use avr_device::{atmega168::Peripherals, interrupt::{self, Mutex}};

static LED_STATE: Mutex<Cell<bool>> = Mutex::new(Cell::new(true));

// Our panic handler. It hangs forever while making the PD3 pin blink.
// In a real world use-case, this can be used to put the device into a
// fail-state mode (shutting off motors, power, ...)
#[cfg(not(doc))]
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    // disable interrupts - firmware has panicked so no ISRs should continue running
    avr_device::interrupt::disable();

    // get the peripherals so we can access serial and the LED.
    //
    // SAFETY: Because main() already has references to the peripherals this is an unsafe
    // operation - but because no other code can run after the panic handler was called,
    // we know it is okay.
    let dp = unsafe { avr_device::atmega168::Peripherals::steal() };

    loop {
        avr_device::asm::delay_cycles(1_000_000);
        set_test_led(&dp, true);
        avr_device::asm::delay_cycles(1_000_000);
        set_test_led(&dp, false);
    }
}

#[avr_device::interrupt(atmega168)]
fn TIMER0_OVF() {
    // This interrupt should raise every (1024*255)/16MHz s ≈ 0.01s
    // We then count 61 times to approximate 1s.
    // XXX: this is a really bad way to count time

    use core::sync::atomic::{AtomicU8, Ordering::Relaxed};

    static OVF_COUNTER: AtomicU8 = AtomicU8::new(0);
    const ROLLOVER: u8 = 61;

    let ovf = OVF_COUNTER.load(Relaxed);
    if ovf < ROLLOVER {
        OVF_COUNTER.store(ovf + 1, Relaxed);
    } else {
        OVF_COUNTER.store(0, Relaxed);
        interrupt::free(|cs| {
            LED_STATE.borrow(cs).set(!LED_STATE.borrow(cs).get());
        });
    }
}

struct RGB(u8,u8,u8);

const WEISS : RGB = RGB(0xff_u8,0xff_u8,0xff_u8);
const GRÜN : RGB = RGB(0x00_u8,0xff_u8,0x00_u8);
const AUS : RGB = RGB(0x00_u8,0x00_u8,0x00_u8);

fn set_data( dp: &Peripherals, b:bool ) {
    if b {
        dp.PORTC.portc.modify(|_, w| w.pc2().set_bit());
    } else {
        dp.PORTC.portc.modify(|_, w| w.pc2().clear_bit());
    }
}

fn set_clock( dp: &Peripherals, b: bool ) {
    if b {
        dp.PORTC.portc.modify(|_, w| w.pc3().set_bit());
    } else {
        dp.PORTC.portc.modify(|_, w| w.pc3().clear_bit());
    }
}

fn set_test_led( dp: &Peripherals, b: bool ) {
    if b {
        dp.PORTC.portc.modify(|_, w| w.pc4().set_bit());
    } else {
        dp.PORTC.portc.modify(|_, w| w.pc4().clear_bit());
    }
}

fn sende_byte( dp: &Peripherals, b: u8 ) {
    for i in 0..8 {
        let led_state = ((1<<i) & b) != 0;
        set_data(dp, led_state);
        set_clock(dp, true);
        //avr_device::asm::delay_cycles(2);
        set_clock(dp, false);
    }
}

fn abschluss( dp: &Peripherals ) {
    //dp.PORTC.portc.modify(|_, w| w.pc3().bit(true));
    //dp.PORTC.portc.modify(|_, w| w.pc3().bit(false));
    set_clock(dp, false);
    avr_device::asm::delay_cycles(1_000);
}

fn leuchte( dp: &Peripherals, col:&RGB ) {
    for ci in [col.0,col.1,col.2] {
        sende_byte(dp,ci);
    }
}

#[avr_device::entry]
fn main() -> ! {
    let dp = avr_device::atmega168::Peripherals::take().unwrap();

    // As you can see, we use .write() instead of .modify(), so the register
    // will be written value + the modified bits

    // Divide by 1024 -> 16MHz/1024 = 15.6kHz
    dp.TC0.tccr0b.write(|w| w.cs0().prescale_1024());
    // Enable overflow interrupts
    dp.TC0.timsk0.write(|w| w.toie0().set_bit());

    // Make pc2 and pc3 outputs
    // We use .modify() in order not to change the other bits
    dp.PORTC.ddrc.modify(|_, w| w.pc2().set_bit());
    dp.PORTC.ddrc.modify(|_, w| w.pc3().set_bit());
    dp.PORTC.ddrc.modify(|_, w| w.pc4().set_bit());

    // SAFETY: We can enable the interrupts here as we are not inside
    // a critical section.
    unsafe {
        avr_device::interrupt::enable();
    }
    set_test_led(&dp, true);

    let mut counter = 0;
    let mut previous_state: bool = true;
    //sende_byte(&dp,0);
    //avr_device::asm::delay_cycles(1_000_000);
    abschluss(&dp);
    loop {
        let mut led_state: bool = true;
        interrupt::free(|cs| {
            //led_state = LED_STATE.borrow(cs).get();
        });

        //dp.PORTC.portc.modify(|_, w| w.pc3().bit(led_state));

        if led_state {
            leuchte(&dp,&WEISS);
        } else {
            leuchte(&dp,&AUS);
        }
        //sende_byte(&dp,0);
        abschluss(&dp);
        //dp.PORTC.portc.modify(|_, w| w.pc4().set_bit());
        //set_test_led(&dp, led_state);
        avr_device::asm::delay_cycles(2_000);

        // We want to make the program crash after 9 blinks
        if previous_state != led_state {
            counter += 1;
        }
        if counter > 999 {
            // The following panics, but it could also be
            // a more "general" bug like dividing by zero, out of bounds,
            // etc..
            panic!();
        }
        led_state = !led_state;
        previous_state = led_state;
    }
}
