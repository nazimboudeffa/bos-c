#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_mem(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("Memoire :\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("  Kernel charge a : 0x10000\n");
    kputs("  Stack            : 0x90000\n");
    kputs("  VGA Buffer       : 0xB8000\n");
    kputs("  Memoire de base  : 640 KB (conventionnelle)\n");
}
