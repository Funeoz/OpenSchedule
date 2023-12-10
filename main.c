#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "partie2/search.h"
#include "partie3/agenda.h"
#include "partie3/complexity.h"
#include "partie3/menu.h"

int running = 1;

void read_exec_command(command* cmd, t_agenda_list *agenda_list) {
    char name[50] = {};
    for (int i = 0; cmd->name[i] != '\0'; i++) {
        name[i] = cmd->name[i];
    }
    if (strcmp(name, "exit") == 0) {
        running = 0;
    } if (strcmp(name, "clear") == 0) {
        clear();
    } else if (strcmp(name, "appointments") == 0) { // Afficher les rendez-vous d’un contact ;
        display_appointment_command(cmd, agenda_list);
    } else if (strcmp(name, "help") == 0) { // afficher l'aide
        display_help();
    } else if (strcmp(name, "create") == 0) {
        if (cmd->params_size == 0) {
            printf("\nCommande invalide");
        } else {
            if (strcmp(cmd->params[0], "appointment") == 0) { // Créer un rendez-vous pour un contact (avec insertion dans la liste si le contact n’existe pas) ;
                create_appointement_command(cmd, agenda_list);
            } else if (strcmp(cmd->params[0], "contact") == 0) { // Créer un contact (avec insertion dans la liste) ;
                create_contact_command(cmd, agenda_list);
            } else {
                printf("Commande invalide\n");
            }
        }
    } else if (strcmp(name, "search") == 0) { // Rechercher un contact, et proposer une complétion automatique à partir de la 3ème lettre entrée pour le nom (il faudra donc faire la saisie du nom de recherche caractère par caractère) ;
        search_contact_command(agenda_list);
    } else if (strcmp(name, "delete") == 0) { // Supprimer un rendez-vous ;
        delete_appointment_command(cmd, agenda_list);
    } else if (strcmp(name, "save") == 0) { // Sauvegarder le fichier de tous les rendez-vous ;
        printf("\nEntrez le chemin du fichier : ");
        char* file_path = scanString();
        save_appointments_in_file(agenda_list, file_path);
    } else if (strcmp(name, "load") == 0) { // Charger un fichier de rendez-vous ;
        load_file_command(cmd, agenda_list);
    } else if (strcmp(name, "timer") == 0) {
        // Fournir les temps de calcul pour une insertion de nouveau contact : voir point 2) ci –dessous.
        timer_command();
    } else if (strcmp(name, "display") == 0) {
        // fonction d'affichage de la liste de test (non affichée dans l'aide)
        display_agenda_list(agenda_list);
    } else {
        printf("\nCommande inconnue");
    }
}

int main() {
    printf(" _____                  _____      _              _       _      \n|  _  |                /  ___|    | |            | |     | |     \n| | | |_ __   ___ _ __ \\ `--.  ___| |__   ___  __| |_   _| | ___ \n| | | | '_ \\ / _ \\ '_ \\ `--. \\/ __| '_ \\ / _ \\/ _` | | | | |/ _ \n\\ \\_/ / |_) |  __/ | | /\\__/ / (__| | | |  __/ (_| | |_| | |  __/\n \\___/| .__/ \\___|_| |_\\____/ \\___|_| |_|\\___|\\__,_|\\__,_|_|\\___|\n      | |                                                        \n      |_|                                                        \n");
    
    // Création de la liste
    t_agenda_list *agenda_list = create_agenda_list(4);

    // Console
    while (running) {
        command *cmd = create_commande();
        cmd->params_size = 0;
        read_from_stdin(cmd);
        read_exec_command(cmd, agenda_list);
    } 
    return 0;
}