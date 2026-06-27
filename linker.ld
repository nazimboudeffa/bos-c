ENTRY(_start)

SECTIONS {
    . = 0x10000;

    .text : {
        kernel/entry.o(.text)   /* point d'entrée garanti en premier */
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
}
