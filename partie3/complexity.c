#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"
#include "../timer.h"
#include "../utils.h"
#include "complexity.h"

// mettre à jour le chemin absolu jusqu'au fichier contacts.csv
static const char FIRST_NAME_FILE[] = "..\\partie3\\prenoms.csv";
static const char LAST_NAME_FILE[] = "..\\partie3\\noms.txt";

// Crée la liste à partir de la liste de contacts dans le fichier csv
t_agenda_list *create_complexity_list() {
    t_agenda_list *list = create_agenda_list(4);
    FILE *first_name_file = fopen(FIRST_NAME_FILE, "r");
    FILE *last_name_file = fopen(LAST_NAME_FILE, "r");
    char first_name[1024];
    char last_name[1024];
    while (fgets(first_name, 1024, first_name_file) && fgets(last_name, 1024, last_name_file)) {
        // on crée la liste de test à partir des deux fichiers, la boucle 
        // s'arrete dès lors que le fichier le plus court est terminé
        first_name[strcspn(first_name, "\n")] = 0;
        last_name[strcspn(last_name, "\n")] = 0;
        insert_in_agenda_all_levels(list, first_name, last_name, 1);
    }
    fclose(first_name_file);
    fclose(last_name_file);
    return list;
}

// Crée un tableau de contact à partir du fichier csv
char *random_from_files() {
    char first_name[1024];
    char last_name[1024];
    int nb_first_names = 36047;
    int nb_last_names = 218982;
    int random_first_name_line = rand() % nb_first_names;
    int random_last_name_line = rand() % nb_last_names;
    FILE *first_name_file = fopen(FIRST_NAME_FILE, "r");
    FILE *last_name_file = fopen(LAST_NAME_FILE, "r");
    for (int i = 0; i < random_first_name_line; i++) {
        // on récupère le prénom aléatoire dans le fichier
        fgets(first_name, 1024, first_name_file);
    }
    for (int i = 0; i < random_last_name_line; i++) {
        // on récupère le nom aléatoire dans le fichier
        fgets(last_name, 1024, last_name_file);
    }
    first_name[strcspn(first_name, "\n")] = 0;
    last_name[strcspn(last_name, "\n")] = 0;
    fclose(first_name_file);
    fclose(last_name_file);
    char *full_name = createFullName(first_name, last_name);
    return full_name;
}

// Effectue les recherches et les écrit dans le fichier log
void test_searches(int nb_searches) {
    printf("En cours...\n");
    char path[256]; // Buffer to hold the path
    char *base_path = "..\\partie3\\results\\recherche\\log";
    char *end_path = ".txt";
    sprintf(path, "%s%d%s", base_path, nb_searches, end_path);
    FILE *log_file = fopen(path, "w");
    char *time_lvl0;
    char *time_all_levels;
    char format[] = "%d searches \t%s\t%s\n";
    t_agenda_list *list = create_complexity_list();
    t_agenda_cell *temp = NULL;

    printf("Nombre de recherches : %d\n", nb_searches);
    
    // chronométrage du temps de recherche au niveau 0
    startTimer();
    for (int j = 0; j < nb_searches; j++) {
        temp = search_agenda_lvl0(list, random_from_files());
    }
    stopTimer();
    printf("Recherche niveau 0 : ");
    displayTime();
    time_lvl0 = getTimeAsString();
    // chronométrage du temps de recherche depuis le niveau 3
    startTimer(); 
    for (int k = 0; k < nb_searches; k++) {
        temp = search_agenda_all_levels(list, random_from_files());
    }
    stopTimer();
    printf("Recherche tous niveaux : ");
    displayTime();
    time_all_levels = getTimeAsString();
    fprintf(log_file,format,nb_searches, time_lvl0, time_all_levels);
    free(list);
    fclose(log_file);
}

void test_insert(int nb_insertions) {
    printf("En cours...\n");
    t_agenda_list *list_lvl0 = create_agenda_list(4);
    t_agenda_list *list_all_levels = create_agenda_list(4);

    // mettre à jour le chemin absolu jusqu'au dossier insertion
    char *base_path = "..\\partie3\\results\\insertion\\log";
    char *end_path = ".txt";
    char path[256]; // Buffer to hold the path
    sprintf(path, "%s%d%s", base_path, nb_insertions, end_path);
    FILE *log_file = fopen(path, "w");
    char *time_lvl0;
    char *time_all_levels;
    char format[] = "%d insertions \t%s\t%s\n";

    printf("Nombre d'insertions : %d\n", nb_insertions);
    
    // chronométrage du temps d'insertion au niveau 0
    startTimer();
    char *full_name= NULL;
    char *last_name = NULL;
    char *first_name = NULL;
    for (int j = 0; j < nb_insertions; j++) {
        full_name = random_from_files();
        last_name = strtok(full_name, "_");
        first_name = strtok(NULL, "_");
        insert_in_agenda_lvl0(list_lvl0, first_name, last_name, 1);
    }
    stopTimer();
    free(list_lvl0);
    printf("Insertion niveau 0 : ");
    displayTime();
    time_lvl0 = getTimeAsString();
    // chronométrage du temps d'insertion depuis le niveau 3
    startTimer(); 
    for (int k = 0; k < nb_insertions; k++) {
        full_name = random_from_files();
        last_name = strtok(full_name, "_");
        first_name = strtok(NULL, "_");
        insert_in_agenda_all_levels(list_all_levels, last_name, first_name, 1);
    }
    stopTimer();
    printf("Insertion tous niveaux : ");
    displayTime();
    time_all_levels = getTimeAsString();
    fprintf(log_file,format, nb_insertions, time_lvl0, time_all_levels);
    free(list_all_levels);
    fclose(log_file);
}