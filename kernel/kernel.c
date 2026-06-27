#include "kernel.h"
#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

/* ---- Utilitaires string (pas de libc) ---- */
int kstrlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

int kstrcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

int kstrncmp(const char *a, const char *b, int n) {
    while (n-- && *a && (*a == *b)) { a++; b++; }
    if (n < 0) return 0;
    return (unsigned char)*a - (unsigned char)*b;
}

char *kstrcpy(char *dst, const char *src) {
    char *d = dst;
    while ((*d++ = *src++));
    return dst;
}

void *kmemset(void *ptr, int val, size_t n) {
    unsigned char *p = (unsigned char *)ptr;
    while (n--) *p++ = (unsigned char)val;
    return ptr;
}

void kputchar(char c) {
    vga_putchar(c);
}

void kputs(const char *str) {
    vga_puts(str);
}

/* ---- Point d'entrée du kernel ---- */
void kmain(void) {
    vga_init();
    keyboard_init();

    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("  ____   ___  ____  \n");
    kputs(" | __ ) / _ \\/ ___| \n");
    kputs(" |  _ \\| | | \\___ \\ \n");
    kputs(" | |_) | |_| |___) |\n");
    kputs(" |____/ \\___/|____/ \n");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kputs("\n  Basic Operating System v0.1\n");
    kputs("  Inspire de MS-DOS - Build 2024\n\n");

    shell_run();

    while (1) {}
}
