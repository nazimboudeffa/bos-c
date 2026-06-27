[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Affiche "BOS Booting..." en mode réel
    mov si, boot_msg
    call print_string_16

    ; Charge le kernel depuis le disque (secteurs 2 à 32)
    mov ax, 0x1000      ; segment de destination
    mov es, ax
    xor bx, bx          ; offset 0

    mov ah, 0x02        ; fonction BIOS: lire secteurs
    mov al, 63          ; nombre de secteurs à lire
    mov ch, 0           ; cylindre 0
    mov cl, 2           ; secteur 2 (le kernel commence ici)
    mov dh, 0           ; tête 0
    mov dl, 0x80        ; premier disque dur (ou 0x00 pour floppy)
    int 0x13
    jc disk_error

    ; Passe en mode protégé 32-bit
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

disk_error:
    mov si, disk_err_msg
    call print_string_16
    hlt

print_string_16:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Saute au kernel C (chargé à 0x10000)
    jmp 0x10000

; ---- GDT ----
gdt_start:
    dq 0x0000000000000000   ; null descriptor
    dq 0x00CF9A000000FFFF   ; code segment
    dq 0x00CF92000000FFFF   ; data segment
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

boot_msg:    db "BOS Booting...", 0x0D, 0x0A, 0
disk_err_msg: db "Disk Error!", 0x0D, 0x0A, 0

times 510 - ($ - $$) db 0
dw 0xAA55
