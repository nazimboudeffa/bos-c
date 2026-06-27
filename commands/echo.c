#include "commands.h"
#include "../kernel/kernel.h"

void cmd_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        kputs(argv[i]);
        if (i < argc - 1) kputchar(' ');
    }
    kputchar('\n');
}
