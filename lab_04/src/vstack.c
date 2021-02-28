#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


#include "defines.h"
#include "vstack.h"

#define START_CAPACITY 8

VStack *vstack_create(size_t size)
{
    VStack *vstack = calloc(1, sizeof(VStack));

    vstack->ptr = NULL;
    vstack->size = size;
    vstack->capacity = START_CAPACITY;
    vstack->buf = calloc(vstack->capacity, sizeof(stacktype_t));

    return vstack;
}

int vstack_push(VStack *vstack, stacktype_t data)
{
    if (vstack->ptr) {
        if ((size_t)(vstack->ptr - vstack->buf + 1) == vstack->size) {
            return OVERFLOW;
        }

        if ((size_t)(vstack->ptr - vstack->buf + 1) == vstack->capacity) {
            vstack_extend_buf(vstack);
        }
    }

    vstack->ptr = vstack->ptr ? vstack->ptr + 1 : vstack->buf;
    *(vstack->ptr) = data;


    return OK;
}

stacktype_t vstack_peek(VStack *vstack, int *rc)
{
    stacktype_t data = 0;
    *rc = EMPTY_STACK;


    if (vstack->ptr) { 
        *rc = OK;
        data = *(vstack->ptr); 
    }

    return data;
}

stacktype_t vstack_pop(VStack *vstack, int *rc)
{
    stacktype_t data = 0;
    *rc = UNDERFLOW;

    if (vstack->ptr) {
        *rc = OK;
        data = *(vstack->ptr);
        vstack->ptr = vstack->ptr != vstack->buf ? vstack->ptr - 1 : NULL;
    }

    return data;
}


int vstack_display(VStack *vstack)  
{  
    if (!(vstack->ptr)) {
        return EMPTY_STACK;
    }

    size_t len = (size_t)(vstack->ptr - vstack->buf + 1);

    printf(ANSI_COLOR_YELLOW"\n[Последний элемент]<--------[Направление стека]-------->[Первый элемент]\n" ANSI_COLOR_GREEN);

    for (size_t i = len; i > 0; i--) {
        printf(" -> %"STYPE_SPEC" ", vstack->buf[i - 1]);
    }

    printf(ANSI_COLOR_RESET"\n");


    return OK;
}  


bool vstack_is_empty(VStack *vstack)
{   
    return !(vstack->ptr);
}


void vstack_delete(VStack *vstack)
{
    free(vstack->buf);
    free(vstack);   
}

void vstack_extend_buf(VStack *this) {
    int pos = this->ptr - this->buf;

    this->capacity = (2 * this->capacity < this->size) ? 2 * this->capacity : this->size;
    this->buf = realloc(this->buf, this->capacity * sizeof(stacktype_t));
    
    this->ptr = this->buf + pos;
}


size_t get_vstack_size(size_t capacity)
{
    return sizeof(VStack *) + sizeof(VStack) + capacity * sizeof(stacktype_t);
}