#include <stdio.h>
#include <stdlib.h>
#include "../include/jemalloc.h"
int main(){
    void * ptr=je_malloc(100);
    je_free(ptr);
}