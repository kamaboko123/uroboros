#![no_main]
#![feature(restricted_std)]

use core::alloc::GlobalAlloc;
use core::alloc::Layout;
use core::ffi::c_uint;

extern "C" {
    fn kvmalloc(size: c_uint) -> c_uint;
    fn kvfree(addr: c_uint);
}


struct UrobosAllocator;

#[global_allocator]
static UROBOROS_ALLOCATOR: UrobosAllocator = UrobosAllocator;

unsafe impl GlobalAlloc for UrobosAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let size = layout.size();
        kvmalloc(size as c_uint) as *mut u8
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        kvfree(ptr as c_uint);
    }
}

#[no_mangle]
pub extern "C" fn rust_func(x:u32, y:u32) -> u32 {
    let mut str = String::from("2");
    str.push_str("2");
    let z:u32 = str.parse().unwrap();
    x + y + z
}
