
out_src_file = "./kernel/asm/asm_interrupts.s"
out_header_file = "./include/asm_interrupts.h"
max_interrupt_num = 0xff

if __name__ == "__main__":
    with open(out_src_file, "w") as f:
        f.write("extern int_handler\n")

    for i in range(max_interrupt_num + 1):
        out = []
        out.append("global int%02x_handler" % i)
        out.append("int%02x_handler:" % i)
        insert_dummy_error_code = not (i in [8, 10, 11, 12, 13, 14])
        if insert_dummy_error_code:
            out.append("    push 0")
        out.append("    push 0x%02x" % i)
        out.append("    push ds")
        out.append("    push es")
        out.append("    push fs")
        out.append("    push gs")
        out.append("    pusha")
        out.append("    jmp all_interrupt")

        with open(out_src_file, "a") as f:
            f.write("\n".join(out))
            f.write("\n")
            f.write("\n")

    with open(out_src_file, "a") as f:
        f.write("global all_interrupt\n")
        f.write("global all_interrupt_ret\n")
        f.write("all_interrupt:\n")
        f.write("    call int_handler\n")
        f.write("all_interrupt_ret:\n")
        f.write("    popa\n")
        f.write("    pop gs\n")
        f.write("    pop fs\n")
        f.write("    pop es\n")
        f.write("    pop ds\n")
        f.write("    add esp, 8\n")
        f.write("    iret\n")

    print("generated: %s" % out_src_file)



    with open(out_header_file, "w") as f:
        f.write("#ifndef ASM_INTERRUPT_H\n")
        f.write("#define ASM_INTERRUPT_H\n")
        f.write("void all_interrupt(void);\n")
        f.write("void all_interrupt_ret(void);\n")

        for i in range(max_interrupt_num + 1):
            f.write("void int%02x_handler(void);\n" % i)
        f.write("#endif\n")

    print("generated: %s" % out_header_file)
