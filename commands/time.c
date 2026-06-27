#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_time(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("Heure : ");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("[driver RTC non implemente]\n");
    kputs("Voir docs/roadmap-dos.md pour l'etape IDT + RTC.\n");
}
