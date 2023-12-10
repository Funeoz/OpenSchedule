#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test.h"
#include "list.h"

void test_display_empty_list() {
    printf("Création d'une liste vide à 5 niveaux : \n");
    t_d_list *empty_list = create_list(5);
    display_list_aligned(empty_list);
}

void test_insert_head() {
    t_d_list *empty_list = create_list(5);
    printf("Insertion de 5 éléments : \n");
    for (int i = 0; i < 5; i++) {
        insert_in_head_of_list(empty_list, create_cell(i, 3));
    }
    display_list_aligned(empty_list);
}

void test_hauteur_cell() {
    t_d_cell *cell = create_cell(10, 3);
    printf("Hauteur de la cellule : %d\n", cell->number_of_cell);
}

void test_random_insert() {
    srand(time(NULL));
    t_d_list *empty_list = create_list(5);
    printf("Insertion de 5 éléments aléatoires : \n");
    for (int i = 0; i < 5; i++) {
        insert_in_head_of_list(empty_list, create_cell(rand() % 100, rand() % 5));
    }
    display_list_aligned(empty_list);
}

void test_random_insert_pos() {
    srand(time(NULL));
    t_d_list *empty_list = create_list(5);
    printf("Insertion de 5 éléments aléatoires : \n");
    insert_in_head_of_list(empty_list, create_cell(1244, 5));
    insert_in_head_of_list(empty_list, create_cell(20, 3));
    insert_in_head_of_list(empty_list, create_cell(4, 1));
    insert_in_head_of_list(empty_list, create_cell(544, 2));
    insert_in_head_of_list(empty_list, create_cell(1, 5));

    insert_in_list(empty_list, create_cell(15, 5));
    display_list_aligned(empty_list);
}

void test_display_level() {
    srand(time(NULL));
    t_d_list *empty_list = create_list(5);
    printf("Insertion de 5 éléments aléatoires : \n");
    insert_in_head_of_list(empty_list, create_cell(30, 5));
    insert_in_head_of_list(empty_list, create_cell(20, 4));
    insert_in_head_of_list(empty_list, create_cell(10, 3));
    insert_in_head_of_list(empty_list, create_cell(5, 4));
    insert_in_head_of_list(empty_list, create_cell(1, 5));
    display_list(empty_list);
    printf("Affichage du niveau : \n");
    display_level_list(empty_list, 0);
}


void test_search_val() {
    srand(time(NULL));
    t_d_list *empty_list = create_list(5);
    printf("Insertion de 5 éléments aléatoires : \n");
    insert_in_head_of_list(empty_list, create_cell(30, 5));
    insert_in_head_of_list(empty_list, create_cell(20, 4));
    insert_in_head_of_list(empty_list, create_cell(10, 3));
    insert_in_head_of_list(empty_list, create_cell(5, 4));
    insert_in_head_of_list(empty_list, create_cell(1, 5));
    display_list(empty_list);
    printf("%d", search_in_list(empty_list, 6)->value);
}