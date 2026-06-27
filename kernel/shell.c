#include "shell.h"
#include "kernel.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../commands/commands.h"

#define CMD_MAX_LEN 128
#define MAX_ARGS    16

static char cmd_buf[CMD_MAX_LEN];
static int  cmd_len = 0;

static void shell_prompt(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    kputs("BOS> ");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

static void split_args(char *buf, char **argv, int *argc) {
    *argc = 0;
    char *p = buf;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[(*argc)++] = p;
        while (*p && *p != ' ') p++;
        if (*p) *p++ = '\0';
        if (*argc >= MAX_ARGS) break;
    }
}

static void read_line(void) {
    cmd_len = 0;
    kmemset(cmd_buf, 0, CMD_MAX_LEN);
    while (1) {
        char c = keyboard_getchar();
        if (!c) continue;
        if (c == '\n') {
            kputchar('\n');
            cmd_buf[cmd_len] = '\0';
            return;
        }
        if (c == '\b') {
            if (cmd_len > 0) {
                cmd_len--;
                cmd_buf[cmd_len] = '\0';
                kputchar('\b');
            }
            continue;
        }
        if (cmd_len < CMD_MAX_LEN - 1) {
            cmd_buf[cmd_len++] = c;
            kputchar(c);
        }
    }
}

static void execute_command(void) {
    char *argv[MAX_ARGS];
    int   argc = 0;
    split_args(cmd_buf, argv, &argc);
    if (argc == 0) return;

    for (int i = 0; i < commands_count; i++) {
        if (kstrcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argv, argc);
            return;
        }
    }

    vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    kputs("'");
    kputs(argv[0]);
    kputs("' n'est pas reconnu. Tapez 'help'.\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void shell_run(void) {
    while (1) {
        shell_prompt();
        read_line();
        execute_command();
    }
}
