# Roadmap — Exécuter des programmes DOS

## Prérequis avant d'exécuter un programme DOS

### 1. Interruptions (IDT) — *étape bloquante*

Les programmes DOS communiquent avec l'OS via `INT 21h`. Sans table d'interruptions (IDT), tout appel système crashe le CPU.

```c
void idt_init();                   // charge l'IDT
void isr_install(int n, handler);  // enregistre un handler
void int21h_handler();             // point d'entrée DOS API
```

### 2. Gestion mémoire de base

Pour charger un programme, il faut savoir où en RAM il peut aller.

```c
void *kmalloc(size_t size);  // allocateur simple (bump allocator suffit)
void  kfree(void *ptr);
```

### 3. Driver disque — filesystem FAT12

Aujourd'hui le bootloader lit des **secteurs bruts**. Pour lire `prog.com`, il faut un filesystem.

- **FAT12** — format natif des disquettes MS-DOS, le plus simple à implémenter (~300 lignes)
- Permet `fopen`, `fread` sur l'image disque

### 4. Chargeur de programmes `.COM`

Un `.COM` est un binaire plat chargé à l'offset `0x100` dans un segment — c'est le format le plus simple qui existe.

```
PSP (256 octets à 0x000)   ← Program Segment Prefix (métadonnées DOS)
Code/Data (à partir 0x100) ← le programme lui-même
```

### 5. Sous-ensemble de l'API INT 21h

Les programmes DOS ne font que ça pour parler au système :

| Fonction | Rôle |
|---|---|
| `AH=09h` | Afficher une chaîne |
| `AH=01h` | Lire un caractère clavier |
| `AH=4Ch` | Quitter le programme |
| `AH=3Dh/3Eh` | Ouvrir/fermer un fichier |

---

## Le vrai obstacle : mode réel vs mode protégé

Les programmes DOS sont **16-bit / mode réel**. Le kernel BOS est en **32-bit / mode protégé**. Trois options :

| Option | Complexité | Description |
|---|---|---|
| **Ne pas switcher** | Faible | Supporter uniquement des programmes 32-bit "maison" (pas de vrais .COM DOS) |
| **Virtual 8086 mode** | Élevée | Le CPU exécute du code 16-bit dans une sandbox protégée — c'est ce que faisaient Windows 3.x et DOSEMU |
| **Rester en mode réel** | Moyenne | Réécrire le kernel en 16-bit — simplifie tout mais bloque à 640 KB de RAM |

---

## Ordre d'implémentation recommandé

Chaque étape est indépendante et testable dans QEMU :

```
IDT + IRQ  →  kmalloc  →  FAT12  →  .COM loader  →  INT 21h minimal
```

### Étape 1 — IDT

Fichiers à créer : `kernel/idt.c`, `kernel/idt.h`, `kernel/isr.asm`

```c
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));
```

### Étape 2 — kmalloc (bump allocator)

```c
static uint8_t *heap_ptr = (uint8_t *)0x200000; // après le kernel

void *kmalloc(size_t size) {
    void *p = heap_ptr;
    heap_ptr += size;
    return p;
}
```

### Étape 3 — FAT12

Fichiers à créer : `drivers/fat12.c`, `drivers/fat12.h`

Lit la FAT depuis le secteur 1 de l'image disque, parcourt les entrées de répertoire, charge les clusters d'un fichier en mémoire.

### Étape 4 — Chargeur .COM

```c
void load_com(const char *filename) {
    uint8_t *psp = kmalloc(256 + 65536);
    kmemset(psp, 0, 256);            // PSP vide
    fat12_read(filename, psp + 256); // charge le code à offset 0x100
    void (*entry)(void) = (void (*)(void))(psp + 256);
    entry();
}
```

### Étape 5 — INT 21h

```c
void int21h_handler(struct registers *r) {
    switch (r->ah) {
        case 0x09: kputs((char *)(r->edx)); break; // print string
        case 0x4C: /* retour au shell */      break; // exit
    }
}
```
