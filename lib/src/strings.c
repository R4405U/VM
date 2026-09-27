#include "strings.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


string* new_string_empty(void){
     string* new_string = (string*)malloc(sizeof(string));
     check_null_ptr(new_string, "Unable to allocate memory for string");

     new_string->data = NULL;
     new_string->length = 0;

     return (string*)new_string;
}

string* string_from_file(char* path){
    FILE* fd = fopen(path, "r");
    if(fd == NULL){
        printf("Could not open file %s", path);
        return NULL;
    }

    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char* buffer = malloc(length + 1);
    if(buffer == NULL){
        printf("Could not allocate memory for file %s\n", path);
        fclose(fd);
        return NULL;
    }

    fread(buffer, 1, length, fd);
    buffer[length] = '\0';

    fclose(fd);

    string* file_read = new_string_empty();

    if (file_read == NULL){
        printf("Could not allocate memory for result structure\n");
        free(buffer); // Free the buffer if we can't allocate memory for the result
        return NULL;
    }


    file_read->data = buffer;
    file_read->length = length;
    return file_read;
}



int string_find_in_char_array(string *src, char *arr[], int arr_length){

    for(int i = 0; i < arr_length; i++){
        if(string_char_equals(src, arr[i])){
            return i;
        }
    }
    return -1;
}





string* new_string(const char* str){
    string* new_string = new_string_empty();
    load_char_ptr_string(new_string, str);
    return new_string;
}

void free_string(string* str){
    if (str->data){
        free(str->data);
    }
    free(str);
}

void free_string_list(string* str_arr[], int arr_length){

    check_null_ptr(str_arr, "Passed NULL String Array");

    for(int i = 0; i < arr_length; i++){
        free_string(str_arr[i]);
        str_arr[i] = NULL;
    }
}

void load_char_ptr_string(string* dest, const char* src){
    check_null_ptr(dest, "Passed Null Destination\n");
    dest->data = strdup(src);
    dest->length = strlen(src);

}


bool string_equals(string* str1, string* str2){
    check_null_ptr(str1, "Passed Null String\n");
    check_null_ptr(str2, "Passed Null String\n");

    if(str1->length != str2->length){
        return false;
    }

    if (strcmp(str1->data, str2->data) == 0){
        return true;
    } else {
        return false;
    }
}

bool string_char_equals(string *str, char *c){
    check_null_ptr(str, "Passed Null String\n");

    size_t c_len = strlen(c);


    if(str->length != (int)c_len){
        return false;
    }

    if (strcmp(str->data, c) == 0){
        return true;
    } else {
        return false;
    }
}


bool string_is_digit(string *str){
    check_null_ptr(str, "Passed Null String\n");

    for(int i = 0; i < str->length; i++){
        if(!isdigit((unsigned char)str->data[i])){
            return false;
        }
    }
    return true;
}

