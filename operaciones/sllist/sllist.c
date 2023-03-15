#include "sllist.h"
#include <stdlib.h>
#include <string.h>

void sllist_init(sllist *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
void sllist_destroy(sllist *list) {
    sllist_node *node = list->head;
    while (node != NULL) {
        sllist_node *next = node->next;
        free(node);
        node = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void sllist_add(sllist *list, int key, char *value1, int value2, double value3) {
    sllist_node *node = malloc(sizeof(sllist_node));
    node->key = key;
    node->value1 = strdup(value1);
    node->value2 = value2;
    node->value3 = value3;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

void sllist_remove(sllist *list, int key) {
    sllist_node *node = list->head;
    sllist_node *prev = NULL;
    while (node != NULL) {
        if (node->key == key) {
            if (prev == NULL) {
                list->head = node->next;
            } else {
                prev->next = node->next;
            }
            if (node == list->tail) {
                list->tail = prev;
            }
            free(node);
            list->size--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

sllist_node *sllist_get(sllist *list, int key) {
    sllist_node *node = list->head;
    while (node != NULL) {
        if (node->key == key) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}
int sllist_exist(sllist *list, int key) {
    sllist_node *node = list->head;
    int result = 0;
    while (node != NULL) {
        if (node->key == key) {
            result = 1;
        }
        node = node->next;
    }
    return result;
}

int sllist_set_value(sllist *list, int key, char *value1, int value2, double value3) {
    sllist_node *node = list->head;
    while (node != NULL) {
        if (node->key == key) {
            node->value1 = value1;
            node->value2 = value2;
            node->value3 = value3;
            return 0;
        }
        node = node->next;
    }
    return -1;
}
