#include "common.h"
#include <stdio.h>



void check_null_ptr(void* ptr, const char* msg){
    if(ptr == NULL){
        perror(msg);
        exit(1);
    }
}
