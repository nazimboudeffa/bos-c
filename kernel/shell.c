#include "shell.h"
#include "kernel.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

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

static void cmd_help(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("Commandes disponibles:\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("  help     - Affiche cette aide\n");
    kputs("  clear    - Efface l'ecran\n");
    kputs("  ver      - Version du systeme\n");
    kputs("  echo     - Affiche un message\n");
    kputs("  color    - Change couleur (fg bg: 0-15)\n");
    kputs("  halt     - Eteint le systeme\n");
    kputs("  reboot   - Redemarrage\n");
    kputs("  mem      - Info memoire\n");
}

static void cmd_ver(void) {
    vga_set_color(VGA_COLOR_LIGHT_YELLOW, VGA_COLOR_BLACK);
    kputs("BOS [Basic Operating System] Version 0.1\n");
    kputs("Inspire de MS-DOS - Build 2024\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

static void cmd_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        kputs(argv[i]);
        if (i < argc - 1) kputchar(' ');
    }
    kputchar('\n');
}

static void cmd_color(char **argv, int argc) {
    if (argc < 3) {
        kputs("Usage: color <fg 0-15> <bg 0-15>\n");
        return;
    }
    uint8_t fg = argv[1][0] - '0';
    uint8_t bg = argv[2][0] - '0';
    if (fg > 15 || bg > 15) {
        kputs("Valeur invalide (0-15)\n");
        return;
    }
    vga_set_color(fg, bg);
    kputs("Couleur changee.\n");
}

static void cmd_mem(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("Memoire:\n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("  Kernel charge a : 0x10000\n");
    kputs("  Stack            : 0x90000\n");
    kputs("  VGA Buffer       : 0xB8000\n");
    kputs("  Memoire de base  : 640 KB (conventionnelle)\n");
}

static void cmd_halt(void) {
    vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    kputs("Systeme arrete. Vous pouvez eteindre.\n");
    __asm__ volatile ("cli; hlt");
    while (1) {}
}

static void cmd_reboot(void) {
    kputs("Redemarrage...\n");
    uint8_t tmp;
    __asm__ volatile (
        "1: inb $0x64, %0\n"
        "   testb $0x02, %0\n"
        "   jnz 1b\n"
        "   movb $0xFE, %0\n"
        "   outb %0, $0x64\n"
        : "=a"(tmp)
    );
    while (1) {}
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

    if (kstrcmp(argv[0], "help") == 0)        cmd_help();
    else if (kstrcmp(argv[0], "clear") == 0)  vga_clear();
    else if (kstrcmp(argv[0], "ver") == 0)    cmd_ver();
    else if (kstrcmp(argv[0], "echo") == 0)   cmd_echo(argv, argc);
    else if (kstrcmp(argv[0], "color") == 0)  cmd_color(argv, argc);
    else if (kstrcmp(argv[0], "mem") == 0)    cmd_mem();
    else if (kstrcmp(argv[0], "halt") == 0)   cmd_halt();
    else if (kstrcmp(argv[0], "reboot") == 0) cmd_reboot();
    else {
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        kputs("Commande inconnue: ");
        kputs(argv[0]);
        kputs("\nTapez 'help' pour la liste des commandes.\n");
        vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    }
}

void shell_run(void) {
    while (1) {
        shell_prompt();
        read_line();
        execute_command();
    }
}
