# BOS System Overview

**BOS (Basic Operating System)** est un mini-système d'exploitation écrit en langage C et Assembleur pour fonctionner dans un émulateur comme QEMU, à la manière de MS-DOS.

L'architecture suit des principes clairs mais avancés, passant par plusieurs phases techniques critiques :

## 📁 Structure Générale
*   **Bootloader (`boot/boot.asm`)**: Le point d'entrée initial qui initialise le système et tente de passer le CPU en Protected Mode.
*   **Noyau (`kernel/`)**: Contient la logique centrale, gérant les services OS et l'initialisation finale des pilotes.
*   **Pilotes périphériques (`drivers/`)**: Modules bas niveau (VGA pour l'affichage, Clavier pour les entrées) responsables de toute communication matérielle simulée.
*   **Shell (`kernel/shell.c`)**: La couche d'interface utilisateur qui reçoit les commandes et orchestre leur exécution via le noyau.

## 🛠️ Phase 0: Prérequis et Fondations Techniques (Deep Dive)
Cette phase établit l’environnement de développement :
1.  **Toolchain:** Composé d'un Assembleur (`Nasm`) et d'un Compiler C avancé (GCC/Clang cross-compiled).
2.  **Connaissances Nécessaires:** Maîtrise du mode x86, des adresses physiques vs virtuelles, de la segmentation et surtout de l'utilisation des *Privilege Rings* (Ring 0 pour le noyau, Ring 3 pour les utilisateurs).
3.  **Contvention:** Utilisation d'un **linker script** (`linker.ld`) qui mappe strictement tous les composants au sein de la mémoire virtuelle du système.

## 🔥 Phase 1: Le Bootloader (Initial Entry Point)
L'exécution démarre en *Real Mode* (limité à 512 octets). Le but est d'atteindre le *Protected Mode*.
**Le processus couvre :** 
*   Définir les limites du boot sector et utiliser la signature `\xAA55`.
*   Utiliser l'interruption BIOS (`INT 13h`) pour lire le noyau depuis un disque simulé.
*   Exécuter le saut spécialisé (`JMP : Selector: Address`) qui force le passage au Protected Mode, après avoir configuré la GDT.

## 💾 Phase 2 (Prochainement): Gestion Mémoire & Interruptions
Cette phase détaillera comment le noyau établit un espace de mémoire virtuel sécurisé en utilisant la **Pagination** et gère tous les événements d'E/S via l'**Interrupt Descriptor Table (IDT)**.

## 🖥️ Phase 3 (Après): L'Interface Utilisateur
Nous aborderons enfin le mécanisme des **System Calls** pour garantir que toutes les requêtes matérielles passent par une interface sécurisée du noyau, permettant au shell de fonctionner correctement et en isolation des processus utilisateurs.
Nous pouvons maintenant continuer avec la phase suivante, sans plus avoir à redéfinir l'existence du fichier ! Je suis prêt quand vous l'êtes pour la Phase 2 : Gestion Mémoire & Interruptions.