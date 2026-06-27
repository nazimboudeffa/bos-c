#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

void cmd_type(char **argv, int argc) {
    if (argc < 2) {
        kputs("Usage: type <fichier>\n");
        return;
    }
    vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    kputs("TYPE : filesystem FAT12 non disponible.\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("Implementez drivers/fat12.c pour activer cette commande.\n");
    kputs("Voir docs/roadmap-dos.md - Etape 3.\n");
}
