#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"
#include "../utils.h"

// crée l'agenda de structure t_agenda_list
t_agenda_list *create_agenda_list(int max_level)
{
    t_agenda_list *list = malloc(sizeof(t_agenda_list));
    list->heads = malloc(sizeof(t_agenda_cell *) * max_level);
    for (int i = 0; i < max_level; i++) {
        list->heads[i] = NULL;
    }
    list->max_level = max_level;
    return list;
}

// crée une cellule de structure t_agenda_cell pour une entrée d'agenda
t_agenda_cell *create_agenda_cell(t_agenda_entry *agenda_entry)
{
    t_agenda_cell *cell = malloc(sizeof(t_agenda_cell));
    cell->agenda_entry = agenda_entry;
    cell->next = malloc(sizeof(t_agenda_cell*) * 4); // chaque cellule contient 4 pointeurs vers les cellules suivantes de chaque niveau
    for (int i = 0; i < 4; i++) {
        cell->next[i] = NULL;
    }
    return cell;
}

// créer une entrée d'agenda de structure t_agenda_entry
t_agenda_entry *create_agenda_entry(t_contact contact, t_appointment *appointment)
{
    t_agenda_entry *agenda_entry = malloc(sizeof(t_agenda_entry));
    agenda_entry->contact = contact;
    agenda_entry->appointment = appointment;
    return agenda_entry;
}


// créer un contact de structure t_contact
t_contact create_contact(char *first_name, char *last_name)
{
    t_contact contact;
    contact.first_name = malloc(sizeof(char) * strlen(first_name));
    strcpy(contact.first_name, first_name);
    contact.last_name = malloc(sizeof(char) * strlen(last_name));
    strcpy(contact.last_name, last_name);
    return contact;
}

// crée un rendez-vous de structure t_appointment qui est affecté à une entrée d'agenda
void create_appointment(t_agenda_cell *cell, struct tm date, struct tm duration, char *object)
{
    t_appointment *appointment = malloc(sizeof(t_appointment));
    appointment->date = date;
    appointment->duration = duration;
    appointment->object = malloc(sizeof(char) * strlen(object));
    strcpy(appointment->object, object);
    appointment->next = NULL;
    t_appointment *temp = cell->agenda_entry->appointment;
    appointment->next = temp;
    cell->agenda_entry->appointment = appointment;
}

// crée une date de structure tm
struct tm create_date(int day, int month, int year, int hour, int min)
{
    // la structure tm est définie dans time.h : elle a l'avantage d'avoir des intervalles pour les valeurs
    // par exemple, tm_mday est compris entre 1 et 31, tm_mon entre 0 et 11, etc.
    struct tm date;
    date.tm_mday = day;
    date.tm_mon = month;
    date.tm_year = year;
    date.tm_hour = hour;
    date.tm_min = min;
    return date;
}

// insère une cellule au début de l'agenda, elle est forcément de hauteur maximale
t_agenda_cell *insert_beginning_agenda(t_agenda_list *list, t_agenda_cell *cell)
{
    t_agenda_cell *temp = NULL;
    for (int i=0; i < 4; i++) {
        // pour chaque niveau, on insère la cellule au début de la liste
        // et sa suivante sera l'ancienne tete de chaque niveau
        temp = list->heads[i];
        list->heads[i] = cell;
        cell->next[i] = temp;
    }
    return cell;
}


// affiche la liste (fonction de test pour vérifier la liste non affichée dans l'aide)
void display_agenda_list(t_agenda_list *list) { 
    t_agenda_cell *cell = list->heads[0];
    // Niveau 0
    while (cell != NULL) {
        printf("%s ", cell->agenda_entry->full_name);
        cell = cell->next[0];
    }
    printf("\n");
    // Niveau 1
    cell = list->heads[1];
    while (cell != NULL) {
        printf("%s ", cell->agenda_entry->full_name);
        cell = cell->next[1];
    }
    printf("\n");
    // Niveau 2
    cell = list->heads[2];
    while (cell != NULL) {
        printf("%s ", cell->agenda_entry->full_name);
        cell = cell->next[2];
    }
    printf("\n");
    // Niveau 3
    cell = list->heads[3];
    while (cell != NULL) {
        printf("%s ", cell->agenda_entry->full_name);
        cell = cell->next[3];
    }
    printf("\n");
}

// renvoie le tableau de suggestions des contacts en cherchant dans l'agenda uniquement au niveau 0
t_agenda_cell **search_suggestions(t_agenda_list *list, char *substr)
{
    t_agenda_cell **suggestions = malloc(sizeof(t_agenda_cell *) * 5); // on a au maximum 5 suggestions
    for (int i = 0; i < 5; i++) {
        suggestions[i] = NULL;
    }
    t_agenda_cell *cell = list->heads[0];
    int i = 0;
    while (cell != NULL && i < 5) {
        if (strstr(cell->agenda_entry->full_name, substr) != NULL) {
            suggestions[i] = cell;
            i++;
        }
        cell = cell->next[0];
    }
    return suggestions;
}


// renvoie la cellule recherchée en cherchant dans l'agenda uniquement au niveau 0
t_agenda_cell *search_agenda_lvl0(t_agenda_list *list, char *full_name)
{
    t_agenda_cell *cell = list->heads[0];
    while (cell != NULL && strcmp(cell->agenda_entry->full_name, full_name) != 0) {
        cell = cell->next[0];
    }
    if (cell != NULL && strcmp(cell->agenda_entry->full_name, full_name) == 0) {
        return cell;
    } else {
        return NULL;
    }
}


// renvoie la cellule recherchée en cherchant à partir du niveau 3 puis en remontant
t_agenda_cell *search_agenda_all_levels(t_agenda_list *list, char *full_name)
{
    t_agenda_cell *temp = list->heads[list->max_level - 1];
    int current_level = list->max_level - 1;
    t_agenda_cell *prev = temp;
    for (int i = list->max_level-1; i >= 0; i--) {
        while (current_level >= 0) {
            if (temp != NULL && (strcmp(list->heads[current_level]->agenda_entry->full_name, full_name) == 0 || strcmp(temp->agenda_entry->full_name, full_name) == 0)) {
                // si la valeur est trouvée
                return temp;
            } else if (prev == temp && strcmp(prev->agenda_entry->full_name, full_name) < 0) {
                // si la valeur cherchée est plus grande que la valeur courante
                // on descend d'un niveau
                current_level--;
                temp = temp->next[current_level];
                prev = temp;
            } else if ((prev == temp && strcmp(temp->agenda_entry->full_name, full_name) > 0) || temp == NULL) {
                // si la valeur cherchée est plus petite que la valeur courante
                // on descend d'un niveau
                current_level--;
                if (current_level < 0) {
                    // si on est au niveau 0 et que la valeur n'est pas trouvée
                    return NULL;
                }
                temp = list->heads[current_level];
                prev = temp;
            } else {
                // on continue de parcourir le niveau
                prev = temp;
                temp = temp->next[current_level];
            }
        }
    }
    return NULL;
}

// si on insère une cellule au sein de la liste, recalcule la hauteur de la cellule suivante
void recalculate_height(t_agenda_cell *cell) {
    // on calcule la hauteur de la cellule suivante
    int next_cell_height = 0;
    if (cell->next[3] != NULL) {
        next_cell_height++;
    } 
    if (cell->next[2] != NULL) {
        next_cell_height++;
    }
    if (cell->next[1] != NULL) {
        next_cell_height++;
    }
    // niveau 3
    t_agenda_cell* level_next_cell = cell->next[3];
    if (next_cell_height >= 2 && level_next_cell != NULL && !(cell->agenda_entry->full_name[0] != level_next_cell->agenda_entry->full_name[0])) {
        // si la hauteur de la cellule suivante est > 2 et que le critère du niveau n'est pas respecté,
        // on chaine notre cellule à celle suivant next[3] et on supprime la cellule suivante
        cell->next[3] = level_next_cell->next[3];
        level_next_cell->next[3] = NULL;
    }
    // niveau 2
    level_next_cell = cell->next[2];
    if (next_cell_height >=2 && level_next_cell != NULL && !(cell->agenda_entry->full_name[0] == level_next_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] != level_next_cell->agenda_entry->full_name[1])) {
        // si la hauteur de la cellule suivante est > 2 et que le critère du niveau n'est pas respecté,
        // on chaine notre cellule à celle suivant next[2] et on supprime la cellule suivante
        cell->next[2] = level_next_cell->next[2];
        level_next_cell -> next[2] = NULL;
    }
    // niveau 1
    level_next_cell = cell->next[1];
    if (next_cell_height >=2 && level_next_cell != NULL && !(cell->agenda_entry->full_name[0] == level_next_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] == level_next_cell->agenda_entry->full_name[1] && cell->agenda_entry->full_name[2] != level_next_cell->agenda_entry->full_name[2])) {
        // si la hauteur de la cellule suivante est > 2 et que le critère du niveau n'est pas respecté,
        // on chaine notre cellule à celle suivant next[1] et on supprime la cellule suivante
        cell->next[1] = level_next_cell->next[1];
        level_next_cell -> next[1] = NULL;
    }
}

// insère une cellule dans l'agenda en utilisant le niveau 0
t_agenda_cell *insert_in_agenda_lvl0(t_agenda_list *list, char *first_name, char *last_name, int isTested)
{
    char *full_name = createFullName(first_name, last_name);
    if (list->heads[0] == NULL) { 
        // si la liste est vide on insère la cellule au début
        t_contact contact = create_contact(first_name, last_name);
        t_agenda_entry *agenda_entry = create_agenda_entry(contact, NULL);
        agenda_entry->full_name = full_name;
        t_agenda_cell *cell = create_agenda_cell(agenda_entry);
        insert_beginning_agenda(list, cell);
        if (isTested == 0) {
            printf("Contact inséré\n");
        }
        return cell;
    } else {
        t_agenda_cell *prev = list->heads[0];
        char *name_temp = list->heads[0]->agenda_entry->full_name;
        // on cherche si la cellule existe déjà
        while (prev != NULL && prev->next[0] != NULL && name_temp != NULL && strcmp(name_temp, full_name) < 0) {
            name_temp = prev->next[0]->agenda_entry->full_name;
            prev = prev->next[0];
        }
        if (name_temp != NULL && strcmp(name_temp, full_name) == 0) {
            if (isTested == 0) {
                // si la cellule existe déjà, on ne l'insère pas
                printf("Contact already exists\n");
            }
            return NULL;
        } else {
            t_contact contact = create_contact(first_name, last_name);
            t_agenda_entry *agenda_entry = create_agenda_entry(contact, NULL);
            agenda_entry->full_name = full_name;
            t_agenda_cell *cell = create_agenda_cell(agenda_entry);

            // on insère la cellule sur les différents niveaux en respectant les critères pour chaque niveau
            t_agenda_cell *level_cell = NULL;
            int cell_max_level = 0;
            for (int i = 0; i < 4; i++) {
                level_cell = list->heads[i];
                while (level_cell != NULL && level_cell->next[i] != NULL && strcmp(level_cell->next[i]->agenda_entry->full_name, full_name) < 0) {
                    level_cell = level_cell->next[i];
                }
                if (level_cell != NULL) {
                    // si la premiere lettre de la cellule est différente de la cellule du niveau,
                    // on met forcément sur les 4 niveaux
                    if (cell->agenda_entry->full_name[0] != level_cell->agenda_entry->full_name[0]) {
                        cell->next[i] = level_cell->next[i];
                        level_cell->next[i] = cell;
                    } else {
                        if (i==0) {
                            cell->next[i] = level_cell->next[i];
                            level_cell->next[i] = cell;
                        }
                        else if (i==1) {
                            // au niveau 1, les deux premières lettres doivent etre les memes et la 3eme différente
                            if (cell->agenda_entry->full_name[0] == level_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] == level_cell->agenda_entry->full_name[1] && cell->agenda_entry->full_name[2] != level_cell->agenda_entry->full_name[2]) {
                                cell->next[i] = level_cell->next[i];
                                level_cell->next[i] = cell;
                            }
                        } else if (i==2) {
                            // au niveau 2, la première lettre doit etre la meme et la 2eme différente
                            if (cell->agenda_entry->full_name[0] == level_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] != level_cell->agenda_entry->full_name[1]) {
                                cell->next[i] = level_cell->next[i];
                                level_cell->next[i] = cell;
                                cell_max_level = 2;
                            }
                        } else if (i==3) {
                            // au niveau 3, on chaine selon la première lettre qui doit etre différente entre les cellules
                            if (cell->agenda_entry->full_name[0] != level_cell->agenda_entry->full_name[0]) {
                                cell->next[i] = level_cell->next[i];
                                level_cell->next[i] = cell;
                                cell_max_level = 3;
                            }
                        }
                    }
                }
            }
            if (cell_max_level == 3) {
                // si la cellule est insérée au niveau 3, on doit forcément la mettre sur les niveaux inférieurs
                // même sans respecter les critères
                level_cell = list->heads[2];
                while (level_cell != NULL && level_cell->next[2] != NULL && strcmp(level_cell->next[2]->agenda_entry->full_name, full_name) < 0) {
                    level_cell = level_cell->next[2];

                }
                if (level_cell != NULL) {
                    cell->next[2] = level_cell->next[2];
                    level_cell->next[2] = cell;
                }
                // on doit aussi la mettre au niveau 1
                cell_max_level--;
            }
            if (cell_max_level == 2) {
                // de même si la cellule va au niveau 2, on doit aussi la mettre au niveau 1
                level_cell = list->heads[1];
                while (level_cell != NULL && level_cell->next[1] != NULL && strcmp(level_cell->next[1]->agenda_entry->full_name, full_name) < 0) {
                    level_cell = level_cell->next[1];
                }
                if (level_cell != NULL) {
                    cell->next[1] = level_cell->next[1];
                    level_cell->next[1] = cell;
                }
            }
            // on appelle la fonction recalculate_height pour recalculer la hauteur de la cellule suivante
            recalculate_height(cell);
            if (isTested == 0) {
                printf("Contact inséré\n");
            }
            return cell;
        }
    }
}

// insère la cellule dans l'agenda en utilisant tous les niveaux
t_agenda_cell *insert_in_agenda_all_levels(t_agenda_list *list, char *first_name, char *last_name, int isTested)
{
    char *full_name = createFullName(first_name, last_name);
    if (list->heads[0] == NULL) { 
        // si la liste est vide on insère la cellule au début
        t_contact contact = create_contact(first_name, last_name);
        t_agenda_entry *agenda_entry = create_agenda_entry(contact, NULL);
        agenda_entry->full_name = full_name;
        t_agenda_cell *cell = create_agenda_cell(agenda_entry);
        insert_beginning_agenda(list, cell);
        if (isTested == 0) {
            printf("Contact inséré\n");
        }
        return cell;
    }
    else if (list->heads[0] != NULL && strcmp(list->heads[0]->agenda_entry->full_name, full_name) > 0) {
        // si la liste n'est pas vide et que la cellule à insérer est plus petite que la première cellule de la liste
        // on insère la cellule au début
        t_agenda_cell *temp = list->heads[0];
        t_contact contact = create_contact(first_name, last_name);
        t_agenda_entry *agenda_entry = create_agenda_entry(contact, NULL);
        agenda_entry->full_name = full_name;
        t_agenda_cell *cell = create_agenda_cell(agenda_entry);
        insert_beginning_agenda(list, cell);
        if (temp->agenda_entry->full_name[0] == full_name[0]) {
            // on recalcule la hauteur de la cellule suivante uniquement si la première lettre est la même
            recalculate_height(cell);
        }
        if (isTested == 0) {
            printf("Contact inséré\n");
        }
        return cell;
    } else {
        int level = list->max_level - 1;
        t_agenda_cell *level_cell = list->heads[level];
        // on cherche si la cellule existe déjà
        while (level_cell != NULL && level_cell->next[level] != NULL && strcmp(level_cell->next[level]->agenda_entry->full_name, full_name) < 0) {
            level_cell = level_cell->next[level];
        }
        if (level_cell != NULL && level_cell->next[level] != NULL && strcmp(level_cell->next[level]->agenda_entry->full_name, full_name) == 0) {
            // si la cellule existe déjà, on ne l'insère pas
            printf("Contact already exists\n"); 
            return NULL;
        } else {
            t_contact contact = create_contact(first_name, last_name);
            t_agenda_entry *agenda_entry = create_agenda_entry(contact, NULL);
            agenda_entry->full_name = full_name;
            t_agenda_cell *cell = create_agenda_cell(agenda_entry);

            int is_already_linked = 0;

            for (int i = level; i >= 0; i--) {
                // boucle pour trouver la position de la cellule à inséreur au niveau i
                level_cell = list->heads[i];
                while (level_cell != NULL && strcmp(level_cell->agenda_entry->full_name, full_name) < 0 && level_cell->next[i] != NULL && strcmp(level_cell->next[i]->agenda_entry->full_name, full_name) < 0) {
                    level_cell = level_cell->next[i];
                }
                if (level_cell != NULL) {
                    if (i == 3) {
                        // au niveau 3, on relie deux cellules si la première lettre de leur chaîne est différente
                        if (cell->agenda_entry->full_name[0] != level_cell->agenda_entry->full_name[0]) {
                            cell->next[level] = level_cell->next[level];
                            level_cell->next[level] = cell;
                            is_already_linked = 1;
                        }
                    } 
                    else if (i == 2) {
                        // au niveau 2, on relie deux cellules si la 2eme lettre de leur chaîne est différente
                        if (is_already_linked || cell->agenda_entry->full_name[0] == level_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] != level_cell->agenda_entry->full_name[1]) {
                            cell->next[i] = level_cell->next[i];
                            level_cell->next[i] = cell;
                            is_already_linked = 1;
                        }
                    }
                    else if (i == 1) {
                        // au niveau 1, on relie deux cellules si la 3eme lettre de leur chaîne est différente
                        if (is_already_linked || cell->agenda_entry->full_name[0] == level_cell->agenda_entry->full_name[0] && cell->agenda_entry->full_name[1] == level_cell->agenda_entry->full_name[1] && cell->agenda_entry->full_name[2] != level_cell->agenda_entry->full_name[2]) {
                            cell->next[i] = level_cell->next[i];
                            level_cell->next[i] = cell;
                            is_already_linked = 1;
                        }
                    } else if (i == 0) {
                        // au niveau 0, on insère la cellule sans critère
                        cell->next[i] = level_cell->next[i];
                        level_cell->next[i] = cell;
                    }
                }
            }
            // on appelle la fonction recalculate_height pour recalculer la hauteur de la cellule suivante
            recalculate_height(cell);
            // si la fonction n'est pas utilisée pour le calcul de complexité, on affiche un message
            // pour prévenir l'utilisateur que le contact a bien été inséré
            if (isTested == 0) {
                printf("Contact inséré\n");
            }
            return cell;
        }
    }
}

// liste les rendez-vous d'un contact à partir de sa cellule correspondante
void list_appointment(t_agenda_cell *cell)
{
    t_agenda_entry *agenda_entry = cell->agenda_entry;
    t_contact contact = agenda_entry->contact;
    printf("Contact : %s %s\n", contact.first_name, contact.last_name);
    t_appointment *appointment = agenda_entry->appointment;
    int appointment_number = 1;
    if (agenda_entry->appointment == NULL) {
        printf("Aucun rendez-vous\n");
        return;
    }
    while (appointment != NULL) {
        printf("\nRendez-vous %d :\n", appointment_number);
        printf("- Date : %d/%d/%d\n", appointment->date.tm_mday, appointment->date.tm_mon, appointment->date.tm_year);
        if (appointment->date.tm_min < 10) {
            printf("- Heure : %d:0%d\n", appointment->date.tm_hour, appointment->date.tm_min);
        } else {
            printf("- Heure : %d:%d\n", appointment->date.tm_hour, appointment->date.tm_min);
        }
        if (appointment->duration.tm_min < 10) {
            printf("- Durée : %dh0%d\n", appointment->duration.tm_hour, appointment->duration.tm_min);
        } else {
            printf("- Durée : %dh%d\n", appointment->duration.tm_hour, appointment->duration.tm_min);
        }
        printf("- Objet : %s\n", appointment->object);
        printf("\n");
        appointment = appointment->next;
        appointment_number++;
    }
}

// supprime un rendez-vous d'un contact à partir de sa cellule correspondante
void delete_appointment(t_agenda_cell *cell, int appointment_number) {
    t_appointment *appointment = cell->agenda_entry->appointment;
    t_appointment *prev = NULL;
    int i = 1;
    // comme les rendez-vous sont chainés,
    // on boucle à travers le tableau de rendez-vous jusqu'à trouver le rendez-vous à supprimer
    while (appointment != NULL && i < appointment_number) {
        prev = appointment;
        appointment = appointment->next;
        i++;
    }
    if (appointment == NULL) {
        printf("Rendez-vous introuvable\n");
        return;
    }
    if (prev == NULL) {
        // si le rendez-vous à supprimer est le premier, on met à jour le pointeur vers le premier rendez-vous
        cell->agenda_entry->appointment = appointment->next;
    } else {
        prev->next = appointment->next;
    }
    free(appointment);
}

// enregistre les rendez-vous dans un fichier
void save_appointments_in_file(t_agenda_list *list, char *path) {
    FILE *log_file = fopen(path, "w");
    char format[] = "%s %s %d/%d/%d %d:%d %d:%d %s\n";
    t_agenda_cell *cell = list->heads[0];
    while (cell != NULL) {
        t_agenda_entry *agenda_entry = cell->agenda_entry;
        t_contact contact = agenda_entry->contact;
        t_appointment *appointment = agenda_entry->appointment;
        while (appointment != NULL) {
            // on utilise fprintf pour écrire dans le fichier les rendez-vous
            fprintf(log_file, format, contact.first_name, contact.last_name, appointment->date.tm_mday, appointment->date.tm_mon, appointment->date.tm_year, appointment->date.tm_hour, appointment->date.tm_min, appointment->duration.tm_hour, appointment->duration.tm_min, appointment->object);
            appointment = appointment->next;
        }
        cell = cell->next[0];
    }
    fclose(log_file);
}

// charge les rendez-vous dans une liste à partir d'un fichier
void load_appointments_from_file(t_agenda_list *list, char* file_path) {
    FILE *log_file = fopen(file_path, "r");
    char first_name[50];
    char last_name[50];
    int day;
    int month;
    int year;
    int hour;
    int min;
    int duration_hour;
    int duration_min;
    char object[1000];
    char format[] = "%s %s %d/%d/%d %d:%d %d:%d %[^\n]\n";
    while (fscanf(log_file, format, first_name, last_name, &day, &month, &year, &hour, &min, &duration_hour, &duration_min, object) != EOF) {
        t_agenda_cell *cell = search_agenda_all_levels(list, createFullName(first_name, last_name));
        if (cell == NULL) {
            // si le contact n'existe pas, on l'insère
            cell = insert_in_agenda_all_levels(list, first_name, last_name, 1);
        }
        // on crée le rendez-vous et on l'insère
        struct tm date = create_date(day, month, year, hour, min);
        struct tm duration = create_date(0, 0, 0, duration_hour, duration_min);
        create_appointment(cell, date, duration, object);
    }
}


