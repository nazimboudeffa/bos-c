# BOS — Basic Operating System

Mini OS en C et assembleur, inspiré de MS-DOS, bootable sur QEMU.

## Prérequis

```bash
sudo apt install nasm gcc binutils qemu-system-x86
```

## Compilation & démarrage

```bash
# Compiler
make

# Lancer (mode texte terminal)
make run

# Lancer (fenêtre graphique VGA)
make run-gui
```

## Commandes du shell BOS

| Commande         | Description                        |
|------------------|------------------------------------|
| `help`           | Liste des commandes                |
| `ver`            | Version du système                 |
| `clear`          | Efface l'écran                     |
| `echo <texte>`   | Affiche un texte                   |
| `color <fg> <bg>`| Change la couleur (0-15)           |
| `mem`            | Infos mémoire                      |
| `reboot`         | Redémarre le système               |
| `halt`           | Arrêt complet                      |

## Architecture

```
boot/boot.asm   → MBR 512 octets, passe en mode protégé 32-bit
kernel/kernel.c → Point d'entrée kmain(), utilitaires string
kernel/shell.c  → Shell interactif avec dispatch de commandes
drivers/vga.c   → Texte VGA 80x25, scroll, couleurs, curseur
drivers/keyboard.c → Clavier PS/2 via polling port 0x60
linker.ld       → Kernel chargé à l'adresse physique 0x10000
```

## Inspiré de MS-DOS

- **Bootloader MBR 512 octets** → identique au concept de MS-DOS boot sector
- **Mode protégé 32-bit** → passage depuis le mode réel comme DOS évolué
- **Shell avec dispatch de commandes** → fidèle au `COMMAND.COM` de MS-DOS
- **Drivers VGA + clavier** → équivalents des interruptions BIOS utilisées par DOS
- **Pas de libc** → tout est réécrit from scratch comme dans MS-DOS original
