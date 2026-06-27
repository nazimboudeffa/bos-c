#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

void kmain(void);
void kputs(const char *str);
void kputchar(char c);

int    kstrlen(const char *s);
int    kstrcmp(const char *a, const char *b);
int    kstrncmp(const char *a, const char *b, int n);
char  *kstrcpy(char *dst, const char *src);
void  *kmemset(void *ptr, int val, size_t n);

#endif
