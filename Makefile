# Outils
ASM    = nasm
CC     = gcc
LD     = ld

# Flags
CFLAGS  = -m32 -ffreestanding -fno-builtin -fno-stack-protector \
		  -fno-pie -fno-pic -nostdlib -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# Fichiers sources
KERNEL_SRC  = kernel/kernel.c shell/shell.c \
              drivers/vga.c drivers/keyboard.c
COMMANDS_SRC = $(wildcard commands/*.c)

KERNEL_OBJ  = kernel/entry.o $(KERNEL_SRC:.c=.o) $(COMMANDS_SRC:.c=.o)

# Cible principale
all: bos.img

# Bootloader
boot/boot.bin: boot/boot.asm
	$(ASM) -f bin $< -o $@

# Entry point assembleur du kernel
kernel/entry.o: kernel/entry.asm
	$(ASM) -f elf32 $< -o $@

# Objets C du kernel
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Kernel binaire
kernel.bin: $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

# Image disque finale : boot + kernel (64 secteurs max)
bos.img: boot/boot.bin kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=boot/boot.bin of=$@ conv=notrunc bs=512 count=1
	dd if=kernel.bin of=$@ conv=notrunc bs=512 seek=1

# Lancer avec QEMU
run: bos.img
	qemu-system-x86_64 -drive format=raw,file=bos.img -nographic \
	    -serial mon:stdio

run-gui: bos.img
	qemu-system-x86_64 -drive format=raw,file=bos.img

# Build + nettoyage des intermédiaires (garde uniquement bos.img)
dist: bos.img
	rm -f boot/boot.bin kernel/entry.o $(KERNEL_OBJ) kernel.bin

# Nettoyage complet
clean:
	rm -f boot/boot.bin kernel/entry.o $(KERNEL_OBJ) kernel.bin bos.img

.PHONY: all run run-gui dist clean
