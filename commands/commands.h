#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*cmd_func_t)(char **argv, int argc);

typedef struct {
    const char  *name;
    const char  *help;
    cmd_func_t   func;
} command_t;

/* Déclarations de toutes les commandes */
void cmd_help   (char **argv, int argc);
void cmd_cls    (char **argv, int argc);
void cmd_ver    (char **argv, int argc);
void cmd_echo   (char **argv, int argc);
void cmd_color  (char **argv, int argc);
void cmd_mem    (char **argv, int argc);
void cmd_pause  (char **argv, int argc);
void cmd_date   (char **argv, int argc);
void cmd_time   (char **argv, int argc);
void cmd_dir    (char **argv, int argc);
void cmd_type   (char **argv, int argc);
void cmd_gui    (char **argv, int argc);
void cmd_reboot (char **argv, int argc);
void cmd_halt   (char **argv, int argc);

/* Registre global (défini dans commands.c) */
extern const command_t commands[];
extern const int       commands_count;

#endif
