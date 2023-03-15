#ifndef SLLIST_H
#define SLLIST_H

typedef struct sllist_node {
    int key;
    char *value1;
    int value2;
    double value3;
    struct sllist_node *next;
} sllist_node;

typedef struct sllist {
    sllist_node *head;
    sllist_node *tail;
    int size;
} sllist;

void sllist_init(sllist *list);
void sllist_destroy(sllist *list);
void sllist_add(sllist *list, int key, char *value1, int value2, double value3);
void sllist_remove(sllist *list, int key);
int sllist_exist(sllist *list, int key);
sllist_node *sllist_get(sllist *list, int key);
int sllist_set_value(sllist *list, int key, char *value1, int value2, double value3);
#endif