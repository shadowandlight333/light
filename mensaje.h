#ifndef _MENSAJE_H_
#define _MENSAJE_H_
#define MAX 256

struct message {
    int op;
    int key;
    int key2;
    char value1[MAX];
    int value2;
    double value3;
    int res;
};

#endif