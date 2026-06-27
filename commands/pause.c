#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/keyboard.h"

void cmd_pause(char **argv, int argc) {
    (void)argv; (void)argc;
    kputs("Appuyez sur une touche pour continuer...");
    while (!keyboard_has_input()) {}
    keyboard_getchar();
    kputchar('\n');
}
