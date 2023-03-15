/*functions of the library claves.h*/
// recibo mensaje de la cola, mando cosas, etc
// mensaje que sea lo más genérico para todas las llamadas
// hay que usar respuestas en forma de estructuras
// las colas solo soportan un tipo de datos
// los hilos son detached

// biblioteca dinamica .so es de linux exclusivamente
// 

// llo que hay aquí no está bien
// includes

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../sllist/sllist.h"

void write_list_to_file(sllist *list) {
    FILE *f = fopen("servidor/data.txt", "w");
    sllist_node *node = list->head;
    while (node != NULL) {
        fprintf(f, "%d %s %d %f\n", node->key, node->value1, node->value2, node->value3);
        node = node->next;
    }
    fclose(f);
}

sllist read_list_from_file(void) {
    FILE *f = fopen("servidor/data.txt", "r");
    int key;
    char value1[100];
    int value2;
    double value3;
    sllist list;
    sllist_init(&list);
    
    while (fscanf(f, "%d %s %d %lf", &key, value1, &value2, &value3) != EOF) {
        sllist_add(&list, key, value1, value2, value3);
    }
    fclose(f);
    return list;
}

int init() {
    FILE *f = fopen("servidor/data.txt", "r");
    if (f == NULL) {
        f = fopen("servidor/data.txt", "w");
        fclose(f);
    } else {
        fclose(f);
    }
    return 0;
}

int exist(int key) {
    sllist list = read_list_from_file();
    int result = sllist_exist(&list, key);
    sllist_destroy(&list);
    if (result)
        return 1;
    return 0;
}

int insert_value(int key, char *value1, int value2, double value3) {
    sllist list = read_list_from_file();
    if (sllist_exist(&list, key))
        return -1;
    sllist_add(&list, key, value1, value2, value3);
    write_list_to_file(&list);
    sllist_destroy(&list);
    return 0;
}

int get_value(int key, char *value1, int *value2, double *value3) {
    // escribe los valores de la clave key en value1, value2 y value3
    sllist list = read_list_from_file();
    if (!sllist_exist(&list, key))
        return -1;
    sllist_node *node = sllist_get(&list, key);
    strcpy(value1, node->value1);
    *value2 = node->value2;
    *value3 = node->value3;
    sllist_destroy(&list);
    return 0;
}

int modify_value(int key, char *value1, int value2, double value3) {
    sllist list = read_list_from_file();
    if (!sllist_exist(&list, key))
        return -1;
    sllist_set_value(&list, key, value1, value2, value3);
    write_list_to_file(&list);
    sllist_destroy(&list);
    return 0;
}

int delete_value(int key) {
    sllist list = read_list_from_file();
    if (!sllist_exist(&list, key))
        return -1;
    sllist_remove(&list, key);
    write_list_to_file(&list);
    sllist_destroy(&list);
    return 0;
}

int copy_key(int key1, int key2) {
    // copiar key1 en key2 (tanto si key2 existe como si no)
    // si key1 no existe, return -1 y no hago nada
    sllist list = read_list_from_file();
    if (!sllist_exist(&list, key1))
        return -1;
    sllist_node *node = sllist_get(&list, key1);
    sllist_set_value(&list, key2, node->value1, node->value2, node->value3);
    write_list_to_file(&list);
    sllist_destroy(&list);
    return 0;
}

/*int main(void) {
    init();
    set_value(1, "hola", 2, 3.0);
    set_value(2, "adios", 3, 4.0);
    modify_value(1, "plancha", 2, 5.0);
    char value1[100];
    int value2;
    double value3;
    get_value(1, value1, &value2, &value3);
    printf("%s %d %f", value1, value2, value3);
    set_value(5, "palomitas", 10, 3.14);
    copy_value(1, 5);
}*/