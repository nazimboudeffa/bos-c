#include "keyboard.h"

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

static const char scancode_map[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,   'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,   '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7','8','9','-','4','5','6','+','1','2','3','0','.', 0, 0, 0,
};

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void keyboard_init(void) {
    while (inb(KEYBOARD_STATUS_PORT) & 0x01) {
        inb(KEYBOARD_DATA_PORT);
    }
}

int keyboard_has_input(void) {
    return (inb(KEYBOARD_STATUS_PORT) & 0x01);
}

char keyboard_getchar(void) {
    uint8_t scancode;
    while (!keyboard_has_input()) {}
    scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode & 0x80) return 0;
    if (scancode < 128) return scancode_map[scancode];
    return 0;
}
