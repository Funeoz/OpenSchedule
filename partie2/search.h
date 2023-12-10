#include <stdio.h>
#include <stdlib.h>

#include "../partie1/list.h"

t_d_list *create_search_list(int n);
int search_level_zero(t_d_list *list, int value);
int search_from_highest(t_d_list *list, int value);
void test_search(int nb_searches);