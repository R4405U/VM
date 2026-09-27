#include "types.h"
#include <stdbool.h>

#ifndef STRINGS_H
#define STRINGS_H

string* new_string_empty(void);
string* new_string(const char* str);
string* string_from_file(char* path);

int string_find_in_char_array(string *src, char *arr[], int arr_length);


void free_string(string* str);
void free_string_list(string* str_arr[], int arr_length);
void load_char_ptr_string(string* dest, const char* src);

bool string_equals(string *str1, string *str2);
bool string_char_equals(string *str, char *c);
bool string_is_digit(string *str);



#endif // STRINGS_H
