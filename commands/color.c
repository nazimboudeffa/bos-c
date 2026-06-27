#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"

static int parse_color_val(const char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9') n = n * 10 + (*s++ - '0');
    return n;
}

void cmd_color(char **argv, int argc) {
    if (argc < 3) {
        kputs("Usage: color <fg 0-15> <bg 0-15>\n");
        return;
    }
    int fg = parse_color_val(argv[1]);
    int bg = parse_color_val(argv[2]);
    if (fg > 15 || bg > 15) {
        kputs("Valeur invalide (0-15)\n");
        return;
    }
    vga_set_color((uint8_t)fg, (uint8_t)bg);
    kputs("Couleur changee.\n");
}
