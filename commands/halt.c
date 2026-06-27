#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_halt(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    kputs("Systeme arrete. Vous pouvez eteindre.\n");
    __asm__ volatile ("cli; hlt");
    while (1) {}
}
