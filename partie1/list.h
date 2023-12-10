#ifndef LIST_H
#define LIST_H


typedef struct s_d_cell {
    int value;
    int number_of_cell; // nombre de pointeurs de cellules dans la cellule
    struct s_d_cell **next; // tableau de pointeurs vers les cellules suivantes
} t_d_cell;

typedef struct s_d_list {
    t_d_cell **heads; // tableau de pointeurs vers les têtes de tableaux
    int max_level;
} t_d_list;

t_d_cell *create_cell(int value, int level_number);
t_d_list *create_list(int max_level);

void insert_in_head_of_list(t_d_list *list, t_d_cell *cell);
void insert_in_list(t_d_list *list, t_d_cell *cell);
t_d_cell* search_in_list(t_d_list *list, int value);
void display_level_list(t_d_list *list, int level);
void display_list(t_d_list *list);
void display_list_aligned(t_d_list *list);
int is_value_in_list(t_d_list *list, int value);

int countDigits(int num);

#endif