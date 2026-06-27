#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_reboot(char **argv, int argc) {
    (void)argv; (void)argc;
    kputs("Redemarrage...\n");
    uint8_t tmp;
    __asm__ volatile (
        "1: inb $0x64, %0\n"
        "   testb $0x02, %0\n"
        "   jnz 1b\n"
        "   movb $0xFE, %0\n"
        "   outb %0, $0x64\n"
        : "=a"(tmp)
    );
    while (1) {}
}
