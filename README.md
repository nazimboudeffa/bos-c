# BOS — Basic Operating System

Mini OS en C et assembleur, inspiré de MS-DOS, bootable sur QEMU.

## Prérequis & Installation

### Linux / macOS

```bash
sudo apt install nasm gcc binutils qemu-system-x86
```

### Windows — MSYS2 (recommandé)

La façon la plus simple sous Windows.

**1.** Installe MSYS2 : [https://www.msys2.org](https://www.msys2.org)

**2.** Ouvre le terminal MSYS2 et installe les outils :
```bash
pacman -S nasm mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils make
```

**3.** Installe QEMU for Windows : [https://www.qemu.org/download/#windows](https://www.qemu.org/download/#windows)

**4.** Ajoute QEMU au PATH dans MSYS2 :
```bash
export PATH=$PATH:"/c/Program Files/qemu"
```

C'est tout — `gcc`, `nasm`, `make` et `dd` sont disponibles directement.

### Windows — WSL2

```powershell
# Dans PowerShell en admin
wsl --install
```
Puis dans le terminal Ubuntu :
```bash
sudo apt install nasm gcc binutils qemu-system-x86
```
> Sur Windows 11, WSLg gère l'affichage graphique automatiquement.  
> Sur Windows 10, installe [VcXsrv](https://sourceforge.net/projects/vcxsrv/) et ajoute `export DISPLAY=:0` dans ton `~/.bashrc`.

---

## Compilation & démarrage

```bash
git clone https://github.com/nazimboudeffa/bos-c.git
cd bos-c

# Compiler
make

# Lancer (fenêtre graphique VGA)
make run-gui

# Lancer (mode texte terminal)
make run
```

---

## Commandes du shell BOS

| Commande          | Description                        |
|-------------------|------------------------------------|
| `help`            | Liste des commandes                |
| `ver`             | Version du système                 |
| `clear`           | Efface l'écran                     |
| `echo <texte>`    | Affiche un texte                   |
| `color <fg> <bg>` | Change la couleur (0-15)           |
| `mem`             | Infos mémoire                      |
| `reboot`          | Redémarre le système               |
| `halt`            | Arrêt complet                      |

---

## Architecture

```
boot/boot.asm      → MBR 512 octets, passe en mode protégé 32-bit
kernel/kernel.c    → Point d'entrée kmain(), utilitaires string
kernel/shell.c     → Shell interactif avec dispatch de commandes
drivers/vga.c      → Texte VGA 80x25, scroll, couleurs, curseur
drivers/keyboard.c → Clavier PS/2 via polling port 0x60
linker.ld          → Kernel chargé à l'adresse physique 0x10000
```

---

## Inspiré de MS-DOS

- **Bootloader MBR 512 octets** → identique au concept de MS-DOS boot sector
- **Mode protégé 32-bit** → passage depuis le mode réel comme DOS évolué
- **Shell avec dispatch de commandes** → fidèle au `COMMAND.COM` de MS-DOS
- **Drivers VGA + clavier** → équivalents des interruptions BIOS utilisées par DOS
- **Pas de libc** → tout est réécrit from scratch comme dans MS-DOS original
