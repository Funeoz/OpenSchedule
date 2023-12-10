#include <stdio.h>
#include <stdlib.h>

#include "list.h"


// crée une cellule avec la valeur et la hauteur de la cellule
t_d_cell *create_cell(int value, int level_number) {
    t_d_cell *cell = malloc(sizeof(t_d_cell));
    cell->value = value;
    cell -> number_of_cell = level_number;
    cell->next = malloc(sizeof(t_d_cell*) * level_number);
    for (int i = 0; i < level_number; i++) {
        cell->next[i] = NULL;
    }
    return cell;
}

// crée une liste avec la hauteur donnée
t_d_list *create_list(int max_level) {
    t_d_list *list = malloc(sizeof(t_d_list));
    list->heads = malloc(sizeof(t_d_cell *) * max_level);
    for (int i = 0; i < max_level; i++) {
        list->heads[i] = NULL;
    }
    list->max_level = max_level;
    return list;
}

// cherche si la valeur est dans la liste et renvoie 1 si oui, 0 sinon
int is_value_in_list(t_d_list *list, int value) {
    t_d_cell *cell = list->heads[list->max_level - 1];
    for (int i = list->max_level - 1; i >= 0; i--) {
        while (cell->next[i] != NULL && cell->next[i]->value < value) {
            cell = cell->next[i];
        }
    }
    if (cell->next[0] != NULL && cell->next[0]->value == value) {
        return 1;
    } else {
        return 0;
    }   
}


// cherche la cellule avec la valeur donnée et renvoie un pointeur vers cette cellule
t_d_cell* search_in_list(t_d_list *list, int value) {
    if (is_value_in_list(list, value)) {
        t_d_cell *cell = list->heads[list->max_level - 1];
        for (int i = list->max_level - 1; i >= 0; i--) {
            while (cell->next[i] != NULL && cell->next[i]->value < value) {
                cell = cell->next[i];
            }
        }
        return cell->next[0];
    } else {
        return NULL;
    }
}


// affiche le niveau de la liste donnée par level
void display_level_list(t_d_list *list, int level) {
    t_d_cell *cell = list->heads[level];
    printf("[list head_%d @-]-->", level);
    while (cell != NULL) {
        printf("[ %d|@-]-->", cell->value);
        cell = cell->next[level];
    }
    printf("NULL\n");
}

// affiche la liste
void display_list(t_d_list *list) {
    for (int i = 0; i<list->max_level; i++) {
        display_level_list(list, i);
    }
}

// compte le nombre de chiffres d'un nombre
int countDigits(int num) {
    int count = 0;
    if (num == 0) {
        return 1;
    }
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

// affiche la liste en alignant les valeurs
void display_list_aligned(t_d_list *list) {
    int list_first_level[1000];
    int nb = 0;
    for (int i = 0; i< list->max_level; i++) {
        if (i == 0) { // Si on est au niveau 0 on fait comme display_list
            t_d_cell *cell = list->heads[i];
            printf("[list head_%d @-]-->", i);
            while (cell != NULL) {
                printf("[ %d|@-]-->", cell->value);
                list_first_level[nb++] = cell->value; // On stocke les valeurs de la liste dans un tableau
                cell = cell->next[i];
            }
            printf("NULL\n");
        } else { // Quand on est à un niveau supérieur à 0
            t_d_cell *cell = list->heads[i];
            printf("[list head_%d @-]--", i);
            int j = 0;
            while (j < nb) {
                if (cell != NULL && cell->value == list_first_level[j]) { 
                    // Si la valeur de la cellule est égale à la valeur du tableau alors 
                    // on affiche la cellule car elle est a la bonne position
                    printf(">[ %d|@-]--", cell->value);
                    cell = cell->next[i];
                } else { 
                    // Sinon cela signifie que la cellule n'est pas a la bonne place
                    // Donc on affiche des tirets pour aligner
                    printf("---------");
                    for (int k = 0; k < countDigits(list_first_level[j]); k++) {
                        printf("-");
                    }
                }
                j++;
            }
            printf(">NULL\n");
        }
    }
}

// insère la cellule en tête de liste
void insert_in_head_of_list(t_d_list *list, t_d_cell *cell) {
    int height = cell -> number_of_cell;
    t_d_cell *temp = NULL;
    for (int i=0; i < height; i++) {
        temp = list->heads[i];
        list->heads[i] = cell;
        cell->next[i] = temp;
    }
}

// insère la cellule dans la liste
void insert_in_list(t_d_list *list, t_d_cell *cell) {
    if (list -> heads[0] == NULL) { // si la liste est vide
        insert_in_head_of_list(list, cell);
        return;
    }
    if (cell->value < list->heads[0]->value) { // si la valeur de la cellule est inférieure à la valeur de la tête de liste
        insert_in_head_of_list(list, cell);
    } else {
        t_d_cell *prev = list->heads[0];
        while ((prev->value < cell->value) && (prev->next[0] != NULL) && (prev->next[0]->value < cell->value)) {
            prev = prev->next[0];
        }
        int prev_height = prev->number_of_cell;
        t_d_cell **tab_ptr_prev = malloc(sizeof(t_d_cell*) * prev_height); // tableau des pointeurs de next de prev
        // On stocke les pointeurs de next de prev dans un tableau
        for (int i = 0; i < prev_height; i++) {
            tab_ptr_prev[i] = prev -> next[i];
        }
        int cell_height = cell->number_of_cell;
        // On met à jour les pointeurs de next de prev et de cell
        for (int j = 0; j < cell_height; j++) {
            prev->next[j] = cell;
            cell->next[j] = tab_ptr_prev[j];
        }
        if (cell_height > prev_height) {
            // si la hauteur de cell est supérieure à celle de prev
            // on met à NULL les pointeurs dont les niveaux sont plus grands que prev_height
            for (int h = prev_height; h < cell_height; h++) {
                prev = list->heads[h];
                int cpt = 0;
                while ((prev != NULL) && (prev->next[h] != NULL) && (prev->value < cell->value)) {
                    prev = prev->next[h];
                    cpt++;
                }
                if (cpt == 0 && prev == NULL) { // si le nouveau pointeur est le premier de la liste et que la liste est vide
                    list -> heads[h] = cell;
                    cell->next[h] = NULL;
                } else if ((cpt == 0) && (prev->value > cell->value)) { // si le nouveau pointeur est le premier de la liste
                    t_d_cell *after = list->heads[h]->next[h];
                    list->heads[h] = cell;
                    cell->next[h] = after;
                } else {
                    t_d_cell *after = prev->next[h];
                    prev->next[h] = cell;
                    cell->next[h] = after;
                }
            }
        }
    }
}