#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "utils.h"

#define MAX_INPUT_LENGTH 1024

// prend l'entrée de l'utilisateur
char *scanString() {
    char *str = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    fgets(str, MAX_INPUT_LENGTH, stdin);
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) -1] = '\0';
    }
    return str;
}

char *extract_substring(char *str, int max_number_of_characters) {
    char *substring = malloc(sizeof(char) * max_number_of_characters);
    strncpy(substring, str, max_number_of_characters);
    substring[max_number_of_characters] = '\0';
    return substring;
}

// inverse l'ordre d'une chaine de caractères
char* reverse_order(char *str) {
    char *words[100];
    int i = 0;

    char *word = strtok(str, " ");
    while(word != NULL) {
        words[i] = word;
        i++;
        word = strtok(NULL, " ");
    }

    // Allocate memory for the reversed string
    char *reversed = malloc(1000);  // assuming the reversed string will not exceed 1000 characters
    reversed[0] = '\0';  // initialize the string

    // Concatenate the words in reverse order
    while(i > 0) {
        i--;
        strcat(reversed, words[i]);
        if(i > 0) {
            strcat(reversed, " ");  // add a space between words
        }
    }

    return reversed;
}

// convertit une chaine de caractères en minuscules
char* str_to_lower(char *string) {
    char *str = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    strcpy(str, string);
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

// créer la chaîne de caractères du nom complet au format "nom_prénom"
char* createFullName(char *firstName, char *lastName) {
    char *separator = "_";
    // on utilise calloc pour initialiser la mémoire à 0 et éviter de stocker des valeurs intrus dans la chaine
    char *fullName = calloc(strlen(firstName) + strlen(lastName) + strlen(separator) +1, sizeof(char)); // +1 for the \0
    strcat(fullName, lastName);
    strcat(fullName, separator);
    strcat(fullName, firstName);
    return str_to_lower(fullName);
}

// nettoie la console
void clear() {
    printf("\e[1;1H\e[2J");
}