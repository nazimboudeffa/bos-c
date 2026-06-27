#include "commands.h"
#include "../kernel/kernel.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

/* ---- Helpers de dessin ------------------------------------------------- */

static void gui_fill_row(int row, uint8_t fg, uint8_t bg) {
    vga_set_cursor(row, 0);
    vga_set_color(fg, bg);
    for (int i = 0; i < VGA_WIDTH; i++) vga_putchar(' ');
}

static void gui_text(int row, int col, uint8_t fg, uint8_t bg, const char *s) {
    vga_set_cursor(row, col);
    vga_set_color(fg, bg);
    kputs(s);
}

/* Cadre double trait ; rempli avec bg */
static void gui_box(int row, int col, int w, int h, uint8_t fg, uint8_t bg) {
    /* Bord haut */
    vga_set_cursor(row, col);
    vga_set_color(fg, bg);
    vga_putchar('\xC9');                          /* ╔ */
    for (int i = 1; i < w - 1; i++) vga_putchar('\xCD');  /* ═ */
    vga_putchar('\xBB');                          /* ╗ */
    /* Bords latéraux + remplissage */
    for (int r = 1; r < h - 1; r++) {
        vga_set_cursor(row + r, col);
        vga_set_color(fg, bg);
        vga_putchar('\xBA');                      /* ║ */
        for (int i = 1; i < w - 1; i++) vga_putchar(' ');
        vga_putchar('\xBA');                      /* ║ */
    }
    /* Bord bas */
    vga_set_cursor(row + h - 1, col);
    vga_set_color(fg, bg);
    vga_putchar('\xC8');                          /* ╚ */
    for (int i = 1; i < w - 1; i++) vga_putchar('\xCD');  /* ═ */
    vga_putchar('\xBC');                          /* ╝ */
}

/* ---- Commande ---------------------------------------------------------- */

void cmd_gui(char **argv, int argc) {
    (void)argv; (void)argc;

    /* Bureau (fond bleu) */
    vga_set_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLUE);
    vga_clear();

    /* Barre de menu haut */
    gui_fill_row(0, VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY);
    gui_text(0,  2, VGA_COLOR_LIGHT_CYAN, VGA_COLOR_DARK_GREY, "BOS");
    gui_text(0, 28, VGA_COLOR_WHITE,      VGA_COLOR_DARK_GREY, "BOS Windows 0.1");
    gui_text(0, 68, VGA_COLOR_WHITE,      VGA_COLOR_DARK_GREY, "12:00");

    /* Barre des tâches bas */
    gui_fill_row(24, VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY);
    gui_text(24,  2, VGA_COLOR_BLACK,      VGA_COLOR_LIGHT_GREY, " Demarrer ");
    gui_text(24, 55, VGA_COLOR_LIGHT_GREY, VGA_COLOR_DARK_GREY,  "Q = quitter BOS Windows");

    /* Fenetre principale */
    gui_box(3, 10, 60, 18, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY);

    /* Barre de titre de la fenetre (bleu) */
    for (int i = 11; i < 69; i++) {
        vga_set_cursor(4, i);
        vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
        vga_putchar(' ');
    }
    gui_text(4, 13, VGA_COLOR_WHITE, VGA_COLOR_BLUE, "Poste de travail");
    gui_text(4, 65, VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE, "[X]");

    /* Barre de menus de la fenetre */
    gui_text(6, 12, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY,
             "Fichier   Edition   Affichage   Aide");

    /* Icones texte */
    gui_text( 9, 13, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "[C:\\]");
    gui_text( 9, 22, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "[A:\\]");
    gui_text( 9, 31, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "[Corbeille]");

    /* Roadmap */
    gui_text(13, 12, VGA_COLOR_BLACK,     VGA_COLOR_LIGHT_GREY,
             "Pour aller plus loin (roadmap-dos.md) :");
    gui_text(14, 14, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "1. IDT + interruptions");
    gui_text(15, 14, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "2. Gestion memoire (kmalloc)");
    gui_text(16, 14, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "3. Driver FAT12");
    gui_text(17, 14, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "4. Chargeur .COM");
    gui_text(18, 14, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY, "5. API INT 21h");

    /* Attente de Q */
    char c;
    do { c = keyboard_getchar(); } while (!c || (c != 'q' && c != 'Q'));

    /* Retour au shell */
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_clear();
}
