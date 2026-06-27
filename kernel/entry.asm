[BITS 32]

EXTERN kmain
GLOBAL _start

_start:
    call kmain
    ; kmain ne retourne jamais, mais au cas où :
    cli
.hang:
    hlt
    jmp .hang
