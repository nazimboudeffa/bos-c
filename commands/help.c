#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_help(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("Commandes disponibles :\n\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    for (int i = 0; i < commands_count; i++) {
        kputs("  ");
        kputs(commands[i].name);
        int len = kstrlen(commands[i].name);
        for (int j = len; j < 10; j++) kputchar(' ');
        kputs("- ");
        kputs(commands[i].help);
        kputchar('\n');
    }
    kputchar('\n');
}
