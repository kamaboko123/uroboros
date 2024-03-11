extern int_handler
global int00_handler
int00_handler:
    push 0
    push 0x00
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int01_handler
int01_handler:
    push 0
    push 0x01
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int02_handler
int02_handler:
    push 0
    push 0x02
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int03_handler
int03_handler:
    push 0
    push 0x03
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int04_handler
int04_handler:
    push 0
    push 0x04
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int05_handler
int05_handler:
    push 0
    push 0x05
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int06_handler
int06_handler:
    push 0
    push 0x06
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int07_handler
int07_handler:
    push 0
    push 0x07
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int08_handler
int08_handler:
    push 0x08
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int09_handler
int09_handler:
    push 0
    push 0x09
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0a_handler
int0a_handler:
    push 0x0a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0b_handler
int0b_handler:
    push 0x0b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0c_handler
int0c_handler:
    push 0x0c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0d_handler
int0d_handler:
    push 0x0d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0e_handler
int0e_handler:
    push 0x0e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int0f_handler
int0f_handler:
    push 0
    push 0x0f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int10_handler
int10_handler:
    push 0
    push 0x10
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int11_handler
int11_handler:
    push 0
    push 0x11
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int12_handler
int12_handler:
    push 0
    push 0x12
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int13_handler
int13_handler:
    push 0
    push 0x13
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int14_handler
int14_handler:
    push 0
    push 0x14
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int15_handler
int15_handler:
    push 0
    push 0x15
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int16_handler
int16_handler:
    push 0
    push 0x16
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int17_handler
int17_handler:
    push 0
    push 0x17
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int18_handler
int18_handler:
    push 0
    push 0x18
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int19_handler
int19_handler:
    push 0
    push 0x19
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1a_handler
int1a_handler:
    push 0
    push 0x1a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1b_handler
int1b_handler:
    push 0
    push 0x1b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1c_handler
int1c_handler:
    push 0
    push 0x1c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1d_handler
int1d_handler:
    push 0
    push 0x1d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1e_handler
int1e_handler:
    push 0
    push 0x1e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int1f_handler
int1f_handler:
    push 0
    push 0x1f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int20_handler
int20_handler:
    push 0
    push 0x20
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int21_handler
int21_handler:
    push 0
    push 0x21
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int22_handler
int22_handler:
    push 0
    push 0x22
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int23_handler
int23_handler:
    push 0
    push 0x23
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int24_handler
int24_handler:
    push 0
    push 0x24
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int25_handler
int25_handler:
    push 0
    push 0x25
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int26_handler
int26_handler:
    push 0
    push 0x26
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int27_handler
int27_handler:
    push 0
    push 0x27
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int28_handler
int28_handler:
    push 0
    push 0x28
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int29_handler
int29_handler:
    push 0
    push 0x29
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2a_handler
int2a_handler:
    push 0
    push 0x2a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2b_handler
int2b_handler:
    push 0
    push 0x2b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2c_handler
int2c_handler:
    push 0
    push 0x2c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2d_handler
int2d_handler:
    push 0
    push 0x2d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2e_handler
int2e_handler:
    push 0
    push 0x2e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int2f_handler
int2f_handler:
    push 0
    push 0x2f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int30_handler
int30_handler:
    push 0
    push 0x30
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int31_handler
int31_handler:
    push 0
    push 0x31
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int32_handler
int32_handler:
    push 0
    push 0x32
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int33_handler
int33_handler:
    push 0
    push 0x33
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int34_handler
int34_handler:
    push 0
    push 0x34
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int35_handler
int35_handler:
    push 0
    push 0x35
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int36_handler
int36_handler:
    push 0
    push 0x36
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int37_handler
int37_handler:
    push 0
    push 0x37
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int38_handler
int38_handler:
    push 0
    push 0x38
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int39_handler
int39_handler:
    push 0
    push 0x39
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3a_handler
int3a_handler:
    push 0
    push 0x3a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3b_handler
int3b_handler:
    push 0
    push 0x3b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3c_handler
int3c_handler:
    push 0
    push 0x3c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3d_handler
int3d_handler:
    push 0
    push 0x3d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3e_handler
int3e_handler:
    push 0
    push 0x3e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int3f_handler
int3f_handler:
    push 0
    push 0x3f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int40_handler
int40_handler:
    push 0
    push 0x40
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int41_handler
int41_handler:
    push 0
    push 0x41
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int42_handler
int42_handler:
    push 0
    push 0x42
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int43_handler
int43_handler:
    push 0
    push 0x43
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int44_handler
int44_handler:
    push 0
    push 0x44
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int45_handler
int45_handler:
    push 0
    push 0x45
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int46_handler
int46_handler:
    push 0
    push 0x46
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int47_handler
int47_handler:
    push 0
    push 0x47
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int48_handler
int48_handler:
    push 0
    push 0x48
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int49_handler
int49_handler:
    push 0
    push 0x49
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4a_handler
int4a_handler:
    push 0
    push 0x4a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4b_handler
int4b_handler:
    push 0
    push 0x4b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4c_handler
int4c_handler:
    push 0
    push 0x4c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4d_handler
int4d_handler:
    push 0
    push 0x4d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4e_handler
int4e_handler:
    push 0
    push 0x4e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int4f_handler
int4f_handler:
    push 0
    push 0x4f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int50_handler
int50_handler:
    push 0
    push 0x50
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int51_handler
int51_handler:
    push 0
    push 0x51
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int52_handler
int52_handler:
    push 0
    push 0x52
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int53_handler
int53_handler:
    push 0
    push 0x53
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int54_handler
int54_handler:
    push 0
    push 0x54
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int55_handler
int55_handler:
    push 0
    push 0x55
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int56_handler
int56_handler:
    push 0
    push 0x56
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int57_handler
int57_handler:
    push 0
    push 0x57
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int58_handler
int58_handler:
    push 0
    push 0x58
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int59_handler
int59_handler:
    push 0
    push 0x59
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5a_handler
int5a_handler:
    push 0
    push 0x5a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5b_handler
int5b_handler:
    push 0
    push 0x5b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5c_handler
int5c_handler:
    push 0
    push 0x5c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5d_handler
int5d_handler:
    push 0
    push 0x5d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5e_handler
int5e_handler:
    push 0
    push 0x5e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int5f_handler
int5f_handler:
    push 0
    push 0x5f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int60_handler
int60_handler:
    push 0
    push 0x60
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int61_handler
int61_handler:
    push 0
    push 0x61
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int62_handler
int62_handler:
    push 0
    push 0x62
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int63_handler
int63_handler:
    push 0
    push 0x63
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int64_handler
int64_handler:
    push 0
    push 0x64
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int65_handler
int65_handler:
    push 0
    push 0x65
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int66_handler
int66_handler:
    push 0
    push 0x66
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int67_handler
int67_handler:
    push 0
    push 0x67
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int68_handler
int68_handler:
    push 0
    push 0x68
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int69_handler
int69_handler:
    push 0
    push 0x69
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6a_handler
int6a_handler:
    push 0
    push 0x6a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6b_handler
int6b_handler:
    push 0
    push 0x6b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6c_handler
int6c_handler:
    push 0
    push 0x6c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6d_handler
int6d_handler:
    push 0
    push 0x6d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6e_handler
int6e_handler:
    push 0
    push 0x6e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int6f_handler
int6f_handler:
    push 0
    push 0x6f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int70_handler
int70_handler:
    push 0
    push 0x70
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int71_handler
int71_handler:
    push 0
    push 0x71
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int72_handler
int72_handler:
    push 0
    push 0x72
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int73_handler
int73_handler:
    push 0
    push 0x73
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int74_handler
int74_handler:
    push 0
    push 0x74
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int75_handler
int75_handler:
    push 0
    push 0x75
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int76_handler
int76_handler:
    push 0
    push 0x76
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int77_handler
int77_handler:
    push 0
    push 0x77
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int78_handler
int78_handler:
    push 0
    push 0x78
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int79_handler
int79_handler:
    push 0
    push 0x79
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7a_handler
int7a_handler:
    push 0
    push 0x7a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7b_handler
int7b_handler:
    push 0
    push 0x7b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7c_handler
int7c_handler:
    push 0
    push 0x7c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7d_handler
int7d_handler:
    push 0
    push 0x7d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7e_handler
int7e_handler:
    push 0
    push 0x7e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int7f_handler
int7f_handler:
    push 0
    push 0x7f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int80_handler
int80_handler:
    push 0
    push 0x80
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int81_handler
int81_handler:
    push 0
    push 0x81
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int82_handler
int82_handler:
    push 0
    push 0x82
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int83_handler
int83_handler:
    push 0
    push 0x83
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int84_handler
int84_handler:
    push 0
    push 0x84
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int85_handler
int85_handler:
    push 0
    push 0x85
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int86_handler
int86_handler:
    push 0
    push 0x86
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int87_handler
int87_handler:
    push 0
    push 0x87
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int88_handler
int88_handler:
    push 0
    push 0x88
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int89_handler
int89_handler:
    push 0
    push 0x89
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8a_handler
int8a_handler:
    push 0
    push 0x8a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8b_handler
int8b_handler:
    push 0
    push 0x8b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8c_handler
int8c_handler:
    push 0
    push 0x8c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8d_handler
int8d_handler:
    push 0
    push 0x8d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8e_handler
int8e_handler:
    push 0
    push 0x8e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int8f_handler
int8f_handler:
    push 0
    push 0x8f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int90_handler
int90_handler:
    push 0
    push 0x90
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int91_handler
int91_handler:
    push 0
    push 0x91
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int92_handler
int92_handler:
    push 0
    push 0x92
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int93_handler
int93_handler:
    push 0
    push 0x93
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int94_handler
int94_handler:
    push 0
    push 0x94
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int95_handler
int95_handler:
    push 0
    push 0x95
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int96_handler
int96_handler:
    push 0
    push 0x96
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int97_handler
int97_handler:
    push 0
    push 0x97
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int98_handler
int98_handler:
    push 0
    push 0x98
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int99_handler
int99_handler:
    push 0
    push 0x99
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9a_handler
int9a_handler:
    push 0
    push 0x9a
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9b_handler
int9b_handler:
    push 0
    push 0x9b
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9c_handler
int9c_handler:
    push 0
    push 0x9c
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9d_handler
int9d_handler:
    push 0
    push 0x9d
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9e_handler
int9e_handler:
    push 0
    push 0x9e
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global int9f_handler
int9f_handler:
    push 0
    push 0x9f
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta0_handler
inta0_handler:
    push 0
    push 0xa0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta1_handler
inta1_handler:
    push 0
    push 0xa1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta2_handler
inta2_handler:
    push 0
    push 0xa2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta3_handler
inta3_handler:
    push 0
    push 0xa3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta4_handler
inta4_handler:
    push 0
    push 0xa4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta5_handler
inta5_handler:
    push 0
    push 0xa5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta6_handler
inta6_handler:
    push 0
    push 0xa6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta7_handler
inta7_handler:
    push 0
    push 0xa7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta8_handler
inta8_handler:
    push 0
    push 0xa8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inta9_handler
inta9_handler:
    push 0
    push 0xa9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intaa_handler
intaa_handler:
    push 0
    push 0xaa
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intab_handler
intab_handler:
    push 0
    push 0xab
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intac_handler
intac_handler:
    push 0
    push 0xac
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intad_handler
intad_handler:
    push 0
    push 0xad
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intae_handler
intae_handler:
    push 0
    push 0xae
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intaf_handler
intaf_handler:
    push 0
    push 0xaf
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb0_handler
intb0_handler:
    push 0
    push 0xb0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb1_handler
intb1_handler:
    push 0
    push 0xb1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb2_handler
intb2_handler:
    push 0
    push 0xb2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb3_handler
intb3_handler:
    push 0
    push 0xb3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb4_handler
intb4_handler:
    push 0
    push 0xb4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb5_handler
intb5_handler:
    push 0
    push 0xb5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb6_handler
intb6_handler:
    push 0
    push 0xb6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb7_handler
intb7_handler:
    push 0
    push 0xb7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb8_handler
intb8_handler:
    push 0
    push 0xb8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intb9_handler
intb9_handler:
    push 0
    push 0xb9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intba_handler
intba_handler:
    push 0
    push 0xba
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intbb_handler
intbb_handler:
    push 0
    push 0xbb
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intbc_handler
intbc_handler:
    push 0
    push 0xbc
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intbd_handler
intbd_handler:
    push 0
    push 0xbd
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intbe_handler
intbe_handler:
    push 0
    push 0xbe
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intbf_handler
intbf_handler:
    push 0
    push 0xbf
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc0_handler
intc0_handler:
    push 0
    push 0xc0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc1_handler
intc1_handler:
    push 0
    push 0xc1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc2_handler
intc2_handler:
    push 0
    push 0xc2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc3_handler
intc3_handler:
    push 0
    push 0xc3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc4_handler
intc4_handler:
    push 0
    push 0xc4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc5_handler
intc5_handler:
    push 0
    push 0xc5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc6_handler
intc6_handler:
    push 0
    push 0xc6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc7_handler
intc7_handler:
    push 0
    push 0xc7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc8_handler
intc8_handler:
    push 0
    push 0xc8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intc9_handler
intc9_handler:
    push 0
    push 0xc9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intca_handler
intca_handler:
    push 0
    push 0xca
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intcb_handler
intcb_handler:
    push 0
    push 0xcb
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intcc_handler
intcc_handler:
    push 0
    push 0xcc
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intcd_handler
intcd_handler:
    push 0
    push 0xcd
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intce_handler
intce_handler:
    push 0
    push 0xce
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intcf_handler
intcf_handler:
    push 0
    push 0xcf
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd0_handler
intd0_handler:
    push 0
    push 0xd0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd1_handler
intd1_handler:
    push 0
    push 0xd1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd2_handler
intd2_handler:
    push 0
    push 0xd2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd3_handler
intd3_handler:
    push 0
    push 0xd3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd4_handler
intd4_handler:
    push 0
    push 0xd4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd5_handler
intd5_handler:
    push 0
    push 0xd5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd6_handler
intd6_handler:
    push 0
    push 0xd6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd7_handler
intd7_handler:
    push 0
    push 0xd7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd8_handler
intd8_handler:
    push 0
    push 0xd8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intd9_handler
intd9_handler:
    push 0
    push 0xd9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intda_handler
intda_handler:
    push 0
    push 0xda
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intdb_handler
intdb_handler:
    push 0
    push 0xdb
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intdc_handler
intdc_handler:
    push 0
    push 0xdc
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intdd_handler
intdd_handler:
    push 0
    push 0xdd
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intde_handler
intde_handler:
    push 0
    push 0xde
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intdf_handler
intdf_handler:
    push 0
    push 0xdf
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte0_handler
inte0_handler:
    push 0
    push 0xe0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte1_handler
inte1_handler:
    push 0
    push 0xe1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte2_handler
inte2_handler:
    push 0
    push 0xe2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte3_handler
inte3_handler:
    push 0
    push 0xe3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte4_handler
inte4_handler:
    push 0
    push 0xe4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte5_handler
inte5_handler:
    push 0
    push 0xe5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte6_handler
inte6_handler:
    push 0
    push 0xe6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte7_handler
inte7_handler:
    push 0
    push 0xe7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte8_handler
inte8_handler:
    push 0
    push 0xe8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inte9_handler
inte9_handler:
    push 0
    push 0xe9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intea_handler
intea_handler:
    push 0
    push 0xea
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inteb_handler
inteb_handler:
    push 0
    push 0xeb
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intec_handler
intec_handler:
    push 0
    push 0xec
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global inted_handler
inted_handler:
    push 0
    push 0xed
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intee_handler
intee_handler:
    push 0
    push 0xee
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intef_handler
intef_handler:
    push 0
    push 0xef
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf0_handler
intf0_handler:
    push 0
    push 0xf0
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf1_handler
intf1_handler:
    push 0
    push 0xf1
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf2_handler
intf2_handler:
    push 0
    push 0xf2
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf3_handler
intf3_handler:
    push 0
    push 0xf3
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf4_handler
intf4_handler:
    push 0
    push 0xf4
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf5_handler
intf5_handler:
    push 0
    push 0xf5
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf6_handler
intf6_handler:
    push 0
    push 0xf6
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf7_handler
intf7_handler:
    push 0
    push 0xf7
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf8_handler
intf8_handler:
    push 0
    push 0xf8
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intf9_handler
intf9_handler:
    push 0
    push 0xf9
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intfa_handler
intfa_handler:
    push 0
    push 0xfa
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intfb_handler
intfb_handler:
    push 0
    push 0xfb
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intfc_handler
intfc_handler:
    push 0
    push 0xfc
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intfd_handler
intfd_handler:
    push 0
    push 0xfd
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intfe_handler
intfe_handler:
    push 0
    push 0xfe
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global intff_handler
intff_handler:
    push 0
    push 0xff
    push ds
    push es
    push fs
    push gs
    pusha
    jmp all_interrupt

global all_interrupt
global all_interrupt_ret
all_interrupt:
    call int_handler
all_interrupt_ret:
    popa
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
    iret
