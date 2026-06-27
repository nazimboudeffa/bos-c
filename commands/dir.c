#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_dir(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    kputs("DIR : filesystem FAT12 non disponible.\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("Implementez drivers/fat12.c pour activer cette commande.\n");
    kputs("Voir docs/roadmap-dos.md - Etape 3.\n");
}
