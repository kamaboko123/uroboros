
use crate::*;


struct Bpb{
    jmp_boot: [u8; 3],
    oem_name: [u8; 8],
    bytes_per_sector: u16,
    sectors_per_cluster: u8,
    reserved_sectors: u16,
    num_fats: u8,
    root_entries: u16,
    total_sectors_small: u16,
    media_info: u8,
    sectors_per_fat_small: u16,
    sectors_per_track: u16,
    num_heads: u16,
    hidden_sectors: u32,
    total_sectors_large: u32,
}

struct Rde{
    filename: [u8; 8],
    ext: [u8; 3],
    attr: u8,
    reserved: u8,
    creation_time_tenths: u8,
    creation_time: u16,
    creation_date: u16,
    access_date: u16,
    high_cluster: u16,
    modification_time: u16,
    modification_date: u16,
    low_cluster: u16,
    size: u32,
}

impl Rde{
    unsafe fn print(&self){
        serial_putstr(self.filename.as_ptr());
    }
}

#[no_mangle]
pub extern "C" fn fat_print_rde(rde: *const Rde) {
    unsafe {
        if (*rde).filename[0] == 0x00 {
            return;
        }
        (*rde).print();
    }
}

