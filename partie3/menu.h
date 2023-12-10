#include "agenda.h"

#ifndef MENU_H
#define MENU_H

typedef struct Command {
    char name[50];
    int params_size;
    char* params[10];
} command;

#define MAX_INPUT_LENGTH 1024

void read_from_stdin(command* cmd);
command* create_commande();
void display_appointment_command(command* cmd, t_agenda_list *agenda_list);
void display_help();
void timer_command();
void create_appointement_command(command* cmd, t_agenda_list *agenda_list);
void create_contact_command(command* cmd, t_agenda_list *agenda_list);
void search_contact_command(t_agenda_list *agenda_list);
void delete_appointment_command(command* cmd, t_agenda_list *agenda_list);
void load_file_command(command* cmd, t_agenda_list *agenda_list);

#endif