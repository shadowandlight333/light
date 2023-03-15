#ifndef OPERACIONES_H
 #define OPERACIONES_H
   #include "../sllist/sllist.h"
   int init();
   int set_value(int key, char *value1, int value2, double value3);
   int get_value(int key, char *value1, int *value2, double *value3);
   int modify_value(int key, char *value1, int value2, double value3);
   int delete_value(int key);
   int exist(int key);
   int copy_key(int key1, int key2);
   void write_list_to_file(sllist *list);
   sllist read_list_from_file(void);
#endif 