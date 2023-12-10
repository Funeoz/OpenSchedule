#include <time.h>

#ifndef AGENDA_H
#define AGENDA_H

typedef struct s_contact
{
    char *first_name;
    char *last_name;
} t_contact;

// on utilise la librairie time.h pour stocker la date et l'heure (pratique car elle pose les intervalles à respecter)   
typedef struct s_appointment
{
    struct tm date;
    struct tm duration;
    char *object;// on stocke l'objet du rendez-vous dans un tableau de char
    struct s_appointment *next;// on stocke un pointeur vers le rendez-vous suivant
} t_appointment;

typedef struct s_agenda_entry {
    t_contact contact; // on stocke un pointeur vers le contact et un pointeur vers le premier rendez-vous si existant
    t_appointment *appointment;
    char *full_name;
} t_agenda_entry;

typedef struct s_agenda_cell {
    t_agenda_entry *agenda_entry;
    struct s_agenda_cell **next; // tableau de pointeurs vers les cellules suivantes
} t_agenda_cell;

typedef struct s_agenda_list {
    t_agenda_cell **heads; // tableau de pointeurs vers les têtes de tableaux
    int max_level;
} t_agenda_list;

t_agenda_list *create_agenda_list(int max_level);
t_agenda_cell *create_agenda_cell(t_agenda_entry *agenda_entry);
t_agenda_entry *create_agenda_entry(t_contact contact, t_appointment *appointment);
t_agenda_cell **search_suggestions(t_agenda_list *list, char *substr);
t_contact create_contact(char *first_name, char *last_name);
void create_appointment(t_agenda_cell *cell, struct tm date, struct tm duration, char *object);
struct tm create_date(int day, int month, int year, int hour, int min);
void list_appointment(t_agenda_cell *cell);
void save_appointments_in_file(t_agenda_list *list, char *path);
void load_appointments_from_file(t_agenda_list *list, char* file_path);
void delete_appointment(t_agenda_cell *cell, int appointment_number);

void display_agenda_list(t_agenda_list *list);

void recalculate_height(t_agenda_cell *cell);
t_agenda_cell *insert_in_agenda_lvl0(t_agenda_list *list, char *first_name, char *last_name, int isTested);
t_agenda_cell *insert_in_agenda_all_levels(t_agenda_list *list, char *first_name, char *last_name, int isTested);

t_agenda_cell *search_agenda_all_levels(t_agenda_list *list, char *full_name);
t_agenda_cell *search_agenda_lvl0(t_agenda_list *list, char *full_name);

#endif