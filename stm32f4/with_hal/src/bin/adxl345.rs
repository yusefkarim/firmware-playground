#![no_std]
#![no_main]
#![deny(unsafe_code)]

use stm32f4_playground as _; // Global logger + panicking-behavior
use stm32f4xx_hal::{i2c::I2c, prelude::*, stm32 as device};

const ADXL345_ADDRESS: u8 = 0x53;
// const ADXL345_ADDRESS: u8 = 0x1D;
#[allow(non_camel_case_types)]
#[allow(dead_code)]
enum ADXL345_Reg {
    DEVID = 0x0,   // Device ID
    DATAX0 = 0x32, // X-axis data 0 (6 bytes for X/Y/Z)
}

#[cortex_m_rt::entry]
fn main() -> ! {
    if let (Some(dp), Some(_cp)) = (
        device::Peripherals::take(),
        cortex_m::peripheral::Peripherals::take(),
    ) {
        // Set up the system clock. We want to run at 48MHz for this one.
        let rcc = dp.RCC.constrain();
        let clocks = rcc.cfgr.sysclk(48.mhz()).freeze();

        // Set up I2C - SCL is PB6 and SDA is PB7; they are set to Alternate Function 4
        let gpiob = dp.GPIOB.split();
        let scl = gpiob.pb6.into_alternate_af4().set_open_drain();
        let sda = gpiob.pb7.into_alternate_af4().set_open_drain();
        let mut i2c = I2c::new(dp.I2C1, (scl, sda), 400.khz(), clocks);

        i2c.write(ADXL345_ADDRESS, &[ADXL345_Reg::DEVID as u8])
            .unwrap();
        let mut data = [0];
        i2c.read(ADXL345_ADDRESS, &mut data).unwrap();
        defmt::info!("Device ID: {}", data[0]);
    }
    loop {}
}
