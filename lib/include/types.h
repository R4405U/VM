#ifndef TYPES_H
#define TYPES_H

typedef struct {
    char* data;
    int length;
}string;


typedef struct {
    string* str;
    int capacity;
    int count;
}string_arr;

#endif // TYPES_H
