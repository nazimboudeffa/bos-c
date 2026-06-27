#include "commands.h"

const command_t commands[] = {
    { "help",   "Affiche cette aide",              cmd_help   },
    { "cls",    "Efface l'ecran",                  cmd_cls    },
    { "ver",    "Version du systeme",              cmd_ver    },
    { "echo",   "Affiche un texte",                cmd_echo   },
    { "color",  "Change couleur (fg bg : 0-15)",   cmd_color  },
    { "mem",    "Infos memoire",                   cmd_mem    },
    { "pause",  "Attend une touche",               cmd_pause  },
    { "date",   "Affiche la date",                 cmd_date   },
    { "time",   "Affiche l'heure",                 cmd_time   },
    { "dir",    "Liste les fichiers du disque",    cmd_dir    },
    { "type",   "Affiche le contenu d'un fichier", cmd_type   },
    { "desk",   "Lance BOS Desktop (TUI)",         cmd_desk   },
    { "reboot", "Redemarrage",                     cmd_reboot },
    { "halt",   "Arret complet",                   cmd_halt   },
};

const int commands_count = sizeof(commands) / sizeof(commands[0]);
