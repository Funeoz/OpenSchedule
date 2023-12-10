#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "complexity.h"
#include "agenda.h"
#include "../utils.h"

void read_from_stdin(command* cmd) {
    char input[MAX_INPUT_LENGTH] = {};
    printf("\n> ");
    fgets(input, MAX_INPUT_LENGTH, stdin);

    // split
    char delim[] = " ";
    char *ptr = strtok(input, delim);
    int is_name = 1;

    while (ptr != NULL) {
        if (is_name) {
            if (ptr[strlen(ptr) - 1] == '\n') {
                ptr[strlen(ptr) -1] = '\0';
            }
            strcpy(cmd->name, ptr);
            is_name = 0;
        } else {
            if (ptr[strlen(ptr) - 1] == '\n') {
                ptr[strlen(ptr) -1] = '\0';
            }
            cmd->params[cmd->params_size] = ptr;
            cmd->params_size++;
        }
        ptr = strtok(NULL, delim); // Aller au prochain string
    }\
}

command* create_commande() {
    command *p = (command *) malloc(sizeof(command));
    return p;
}

void display_appointment_command(command* cmd, t_agenda_list *agenda_list) {
    char *first_name;
    char *last_name;
    if (cmd->params_size == 1) {
        first_name = cmd->params[0];
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    } 
    else if (cmd->params_size >= 2) {
        first_name = cmd->params[0];
        last_name = cmd->params[1];
    } else {
        printf("\nEntrez le prénom du contact : ");
        first_name = scanString();
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    }
    char* full_name = createFullName(first_name, last_name);
    t_agenda_cell *cell = search_agenda_all_levels(agenda_list, full_name);
    if (cell != NULL) {
        list_appointment(cell);
    } else {
        printf("Contact introuvable\n");
    }
}

void display_help() {
    printf("\nAIDE :\n[] = obligatoire\t<> : optionnel\n\n- help : afficher l'aide\n"
        "- clear : effacer l'ecran\n"
        "- exit : quitter le programme\n"
        "- appointments <firstName> <lastName> : afficher les rendez-vous d'un contact\n"
        "- create [contact/appointment] : creer un contact ou un rendez-vous\n"
        "- search : rechercher un contact\n"
        "- delete : supprimer un rendez-vous\n"
        "- save : sauvegarder le fichier de tous les rendez-vous\n"
        "- load <filePath> : charger un fichier de rendez-vous\n"
        "- timer : fournir les temps de calcul pour une insertion de nouveau contact\n");
}

void create_appointement_command(command* cmd, t_agenda_list *agenda_list) {
    if (agenda_list->heads[0] == NULL) {
        printf("Aucun contact dans l'agenda\n");
        return;
    }
    char *first_name;
    char *last_name;
    if (cmd->params_size == 2) { // Le premier paramètre est la commande
        first_name = cmd->params[1];
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    } 
    else if (cmd->params_size >= 3) {
        first_name = cmd->params[1];
        last_name = cmd->params[2];
    } else {
        printf("\nEntrez le prénom du contact : ");
        first_name = scanString();
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    }

    char* full_name = createFullName(first_name, last_name);
    t_agenda_cell *cell = search_agenda_all_levels(agenda_list, full_name);
    
    if (cell == NULL) {
        // Créer le contact s'il nexiste pas
        cell = insert_in_agenda_all_levels(agenda_list, first_name, last_name, 0);
    }
    // Objet du rdv
    printf("\nEntrez l'objet du rendez-vous : ");
    char *object = scanString();
    // date
    printf("\nEntrez la date du rendez-vous (jj/mm/aaaa) : ");
    char *date = scanString();
    int day = atoi(strtok(date, "/"));
    int month = atoi(strtok(NULL, "/"));
    int year = atoi(strtok(NULL, "/"));
    // heure et minutes
    printf("\nEntrez l'heure du rendez-vous (hh:mm) : ");
    char *time = scanString();
    int hour = atoi(strtok(time, ":"));
    int min = atoi(strtok(NULL, ":"));
    // création de la date avec la structure tm
    struct tm date_tm = create_date(day, month, year, hour, min);
    // durée du rdv
    printf("\nEntrez la durée du rendez-vous (hh:mm) : ");
    char *duration = scanString();
    int duration_hour = atoi(strtok(duration, ":"));
    int duration_min = atoi(strtok(NULL, ":"));
    struct tm duration_tm = create_date(0, 0, 0, duration_hour, duration_min);
    // création du rendez-vous
    create_appointment(cell, date_tm, duration_tm, object);
}

void create_contact_command(command* cmd, t_agenda_list *agenda_list) {
    char *first_name;
    char *last_name;
    if (cmd->params_size == 2) { // Le premier paramètre est la commande
        first_name = cmd->params[1];
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    } 
    else if (cmd->params_size >= 3) {
        // parsing des paramètres
        first_name = cmd->params[1];
        last_name = cmd->params[2];
    } else {
        printf("\nEntrez le prénom du contact : ");
        first_name = scanString();
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    }
    insert_in_agenda_all_levels(agenda_list, first_name, last_name, 0);
}

void search_contact_command(t_agenda_list *agenda_list) {
    clear();
    char *input = malloc(MAX_INPUT_LENGTH * sizeof(char));
    char* msg = "\nEntrez le nom du contact recherché (tapez \"entrer\" pour voir les suggestions ou \"quit\" pour quitter la recherche): ";
    printf(msg);

    // Exécuter tant que l'utilisateur n'entre pas <Enter>
    while (1) {
        fgets(input, MAX_INPUT_LENGTH, stdin);
        input[strcspn(input, "\n")] = 0; // supprime le \n
        if (strcmp(input, "quit") == 0) {
            clear();
            break;
        }
        else if (strlen(input) >= 3) {
            input = str_to_lower(input);
            clear();
            t_agenda_cell **results = search_suggestions(agenda_list, input);
            if (results[0] == NULL) {
                printf("Aucun suggestion de contact trouvée\n");
                break;
            } else {
                printf("Suggestions de contact pour la recherche \"%s\" :", input);
                for (int i = 0; results[i] != NULL; i++) {
                    printf(" %s %s\t", results[i]->agenda_entry->contact.first_name, results[i]->agenda_entry->contact.last_name);
                }
                printf(msg);
            }
        }
    }
}

void delete_appointment_command(command* cmd, t_agenda_list *agenda_list) {
    if (agenda_list->heads[0] == NULL) {
        printf("Aucun contact dans l'agenda\n");
        return;
    }
    char *first_name;
    char *last_name;
    if (cmd->params_size == 1) { 
        first_name = cmd->params[0];
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    } 
    else if (cmd->params_size >= 2) {
        first_name = cmd->params[0];
        last_name = cmd->params[1];
    } else {
        printf("\nEntrez le prénom du contact : ");
        first_name = scanString();
        printf("\nEntrez le nom du contact : ");
        last_name = scanString();
    }
    char* full_name = createFullName(first_name, last_name);
    t_agenda_cell *cell = search_agenda_all_levels(agenda_list, full_name);
    if (cell != NULL) {
        list_appointment(cell);
        printf("\nEntrez le numéro du rendez-vous à supprimer : ");
        int appointment_number = atoi(scanString());
        delete_appointment(cell, appointment_number);
    } else {
        printf("Contact introuvable\n");
    }
}

void load_file_command(command* cmd, t_agenda_list *agenda_list) {
    if (cmd->params_size == 0) {
        printf("\nEntrez le chemin absolu du fichier : ");
        char* file_path = scanString();
        load_appointments_from_file(agenda_list, file_path);
    } else {
        load_appointments_from_file(agenda_list, cmd->params[0]);
    }
}

void timer_command() {
    printf("Voulez-vous tester la recherche ou l'insertion ? (r/i) : ");
    char *choice = scanString();
    if (strcmp(choice, "r") == 0) {
        int nb_searches_int = 0;
        while (nb_searches_int <= 0 || nb_searches_int > 30000) {
            printf("\nEntrez le nombre de recherches à effectuer : ");
            char *nb_insertions = scanString();
            nb_searches_int = atoi(nb_insertions); 
            if (nb_searches_int <= 0) {
                printf("\nVeuillez entrer un nombre positif entre 1 et 30000");
            }
        }
        test_searches(nb_searches_int);
    } else if (strcmp(choice, "i") == 0) {
        int nb_insertions_int = 0;
        while (nb_insertions_int <= 0 || nb_insertions_int > 30000) {
            printf("\nEntrez le nombre d'insertions à effectuer : ");
            char *nb_insertions = scanString();
            nb_insertions_int = atoi(nb_insertions); 
            if (nb_insertions_int <= 0) {
                printf("\nVeuillez entrer un nombre positif entre 1 et 3000");
            }
        }
        test_insert(nb_insertions_int);
    } else {
        printf("Commande invalide\n");
    }

}