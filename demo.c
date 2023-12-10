#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partie1/list.h"
#include "utils.h"
#include "partie2/search.h"
#include "partie1/test.h"

#define MAX_INPUT_LENGTH 1024

typedef struct Command {
    char name[50];
    int params_size;
    char* params[10];
} command;

int running = 1;

command* create_commande() {
    command *p = (command *) malloc(sizeof(command));
    return p;
}

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

int read_exec_command(command* cmd, t_d_list **liste) {
    char name[50] = {};
    for (int i = 0; cmd->name[i] != '\0'; i++) {
        name[i] = cmd->name[i];
    }
    if (strcmp(name, "exit") == 0) {
        running = 0;
    } else if (strcmp(name, "clear") == 0) {
        clear();
    } else if (strcmp(name, "create_search_list") == 0) {
        printf("Taille de la liste : ");
        int size = 0;
        size = atoi(scanString());
        *liste = create_search_list(size);
    } else if (strcmp(name, "search_level_zero") == 0) {
        printf("Valeur à rechercher : ");
        int value = 0;
        value = atoi(scanString());
        if (search_level_zero(*liste, value)) {
            printf("Valeur trouvée\n");
        } else {
            printf("Valeur non trouvée\n");
        }
    } else if (strcmp(name, "search_from_highest") == 0) {
        printf("Valeur à rechercher : ");
        int value = 0;
        value = atoi(scanString());
        if (search_from_highest(*liste, value)) {
            printf("Valeur trouvée\n");
        } else {
            printf("Valeur non trouvée\n");
        }
    } else if (strcmp(name, "help") == 0) {
        printf("\nListe des commandes de test disponibles : \n"
        "create_search_list : create search list\n"
        "search_level_zero : search level zero\n"
        "search_from_highest : search from highest\n"
        "test_search : test search\n"
        "display : display list\n"
        );
    } else if (strcmp(name, "test_search") == 0) {
        printf("Nombre de recherches : ");
        int nb_searches = 0;
        nb_searches = atoi(scanString());
        test_search(nb_searches);
    } else if (strcmp(name, "display") == 0) {
        display_list_aligned(*liste);
    } else {
        printf("\nCommande inconnue");
    }
}

int main() {
    printf(" _____                  _____      _              _       _      \n|  _  |                /  ___|    | |            | |     | |     \n| | | |_ __   ___ _ __ \\ `--.  ___| |__   ___  __| |_   _| | ___ \n| | | | '_ \\ / _ \\ '_ \\ `--. \\/ __| '_ \\ / _ \\/ _` | | | | |/ _ \n\\ \\_/ / |_) |  __/ | | /\\__/ / (__| | | |  __/ (_| | |_| | |  __/\n \\___/| .__/ \\___|_| |_\\____/ \\___|_| |_|\\___|\\__,_|\\__,_|_|\\___|\n      | |                                                        \n      |_|                                                        \n");
    printf("Bienvenue dans la démonstration de la partie 2\n\n");


    t_d_list *liste = create_search_list(3);
    while (running) {
        command *cmd = create_commande();
        cmd->params_size = 0;
        read_from_stdin(cmd);
        read_exec_command(cmd, &liste);
    } 
    return 0;
}


