#![no_main]
#![no_std]

use core::panic::PanicInfo;

#[panic_handler]
fn my_panic_handler(_info: &PanicInfo) -> ! {
    loop {}
}

#[no_mangle]
extern "C" fn rust_eh_personality() {}

#[no_mangle]
pub extern "C" fn rust_func_add(x:u32, y:u32) -> u32 {
    x + y
}

