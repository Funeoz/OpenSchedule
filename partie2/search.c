#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../partie1/list.h"
#include "../partie1/test.h"
#include "../utils.h"
#include "../timer.h"

// Création d'une liste de taille n
t_d_list *create_search_list(int n) {
    int heads_length = pow(2.0, n)-1;
    t_d_list *list = create_list(n);
    int tab[heads_length];
    for (int i = 0; i < heads_length; i++) {
        tab[i] = 0;
    }
    for (int i = 1; i < n; i++) {
        int step = pow(2.0, i); // pas d'incrémentation de la hauteur de la cellule
        int start = pow(2.0, i)-1; // cellule où démarre l'incrémentation
        for (int j = start; j < heads_length; j+=step) {
            tab[j] += 1;
        }
    }
    // insertion des cellules dans la liste
    for (int k = 0; k < heads_length; k++) {
        insert_in_list(list, create_cell(k+1, tab[k]+1));
    }
    return list;
}

// Recherche d'une valeur uniquement au niveau 0
int search_level_zero(t_d_list *list, int value) {
    t_d_cell *cell = list->heads[0];
    while (cell->value != value && cell->next[0] != NULL) {
        cell = cell->next[0];
    }
    if (cell->value == value) {
        return 1;
    } else {
        return 0;
    }
}

// recherche d'une valeur à partir du niveau le plus haut
int search_from_highest(t_d_list *list, int val) {
    int current_level = list->max_level - 1;
    t_d_cell *temp = list->heads[list->max_level - 1];
    t_d_cell *prev = temp;
    for (int i = list->max_level-1; i >= 0; i--) {
        while (current_level >= 0) {
            if (temp != NULL && (list->heads[current_level]->value == val || temp -> value == val)) {
                // si la valeur est trouvée
                return 1;
            } else if ((prev == temp && prev->value < val)) {
                // si la valeur est plus grande que la valeur de la cellule
                // on descend d'un niveau
                current_level--;
                temp = temp->next[current_level];
                prev = temp;
            } else if (prev == temp && temp -> value > val || temp == NULL) {
                // si la valeur est plus petite que la valeur de la cellule
                // on descend d'un niveau
                current_level--;
                if (current_level < 0) {
                    // si on est au niveau 0 et que la valeur n'est pas trouvée
                    return 0;
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
    return 0;
}


// Test de la recherche
void test_search(int nb_searches) {
    char *base_path = "..\\partie2\\results\\log";
    char *end_path = ".txt";
    char path[256]; // Buffer to hold the path
    sprintf(path, "%s%d%s", base_path, nb_searches, end_path);
    FILE *log_file = fopen(path, "a");
    char *time_lvl0;
    char *time_all_levels;
    char format[] = "%d\t%s\t%s\n";
    printf("Nombre de recherches : %d\n", nb_searches);
    for (int i = 7; i<16; i++) {
        srand(time(NULL)); // initialisation de rand de manière aléatoire
        printf("Niveau %d\n", i);
        t_d_list *list = create_search_list(i);
        startTimer();
        for (int j = 0; j < nb_searches; j++) {
            search_level_zero(list, rand()%1000);
        }
        stopTimer();
        time_lvl0 = getTimeAsString();
        printf("Recherche niveau 0 : \n");
        displayTime();
        printf("\n");
        startTimer();
        for (int k = 0; k < nb_searches; k++) {
            search_from_highest(list, rand()%1000);
        }
        stopTimer();
        time_all_levels = getTimeAsString();
        printf("Recherche tous niveaux : \n");
        displayTime();
        printf("\n");
        fprintf(log_file,format,i, time_lvl0, time_all_levels);
        printf("\n");
        free(list);
    }
    fclose(log_file);
}