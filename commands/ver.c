#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_ver(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_LIGHT_YELLOW, VGA_COLOR_BLACK);
    kputs("BOS [Basic Operating System] Version 0.1\n");
    kputs("Inspire de MS-DOS - Build 2024\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
