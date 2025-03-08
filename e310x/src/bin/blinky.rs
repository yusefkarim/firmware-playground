//! Blinks an LED

#![deny(unsafe_code)]
#![deny(warnings)]
#![no_std]
#![no_main]

use e310x_hal as hal;
use e310x_playground as _;
use hal::{prelude::*, DeviceResources};
use riscv::asm::nop;
use rtt_target;

#[riscv_rt::entry]
fn main() -> ! {
    rtt_target::rtt_init_print!(NoBlockSkip);
    rtt_target::rprintln!("System starting up...");
    // Take ownership of the device resources singleton
    if let Some(dr) = DeviceResources::take() {
        rtt_target::rprintln!("Configuring peripherals...");
        // Configure the LED on GPIO0 pin 5 as an output.
        let mut led = dr.pins.pin5.into_output();

        rtt_target::rprintln!("Entering main loop...");
        loop {
            led.toggle().unwrap();
            rtt_target::rprint!(".");
            for _ in 0..10_000_00 {
                #[allow(unsafe_code)]
                unsafe {
                    nop();
                }
            }
        }
    }
    panic!();
}
