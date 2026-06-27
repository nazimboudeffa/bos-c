#include "commands.h"
#include "../drivers/vga.h"

void cmd_cls(char **argv, int argc) {
    (void)argv; (void)argc;
    vga_clear();
}
