#include "vga.h"

static uint16_t *vga_buffer = (uint16_t *)0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t current_color = 0;

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_update_cursor(void) {
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
    __asm__ volatile (
        "outb %0, $0x3D4\n"
        "outb %1, $0x3D5\n"
        : : "a"((uint8_t)0x0F), "a"((uint8_t)(pos & 0xFF))
    );
    __asm__ volatile (
        "outb %0, $0x3D4\n"
        "outb %1, $0x3D5\n"
        : : "a"((uint8_t)0x0E), "a"((uint8_t)((pos >> 8) & 0xFF))
    );
}

void vga_init(void) {
    current_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}

void vga_clear(void) {
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_entry(' ', current_color);
    }
    cursor_row = 0;
    cursor_col = 0;
    vga_update_cursor();
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    current_color = vga_entry_color(fg, bg);
}

void vga_set_cursor(int row, int col) {
    cursor_row = row;
    cursor_col = col;
    vga_update_cursor();
}

void vga_newline(void) {
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_entry(' ', current_color);
        }
        cursor_row = VGA_HEIGHT - 1;
    }
    vga_update_cursor();
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_newline();
        return;
    }
    if (c == '\r') {
        cursor_col = 0;
        vga_update_cursor();
        return;
    }
    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(' ', current_color);
            vga_update_cursor();
        }
        return;
    }
    vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, current_color);
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        vga_newline();
    } else {
        vga_update_cursor();
    }
}

void vga_puts(const char *str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_puts_color(const char *str, uint8_t fg, uint8_t bg) {
    uint8_t saved = current_color;
    vga_set_color(fg, bg);
    vga_puts(str);
    current_color = saved;
}
