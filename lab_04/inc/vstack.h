#ifndef _VSTACK_H_
#define _VSTACK_H_

#include <stdlib.h>
#include <time.h>
#include "defines.h"


#define DEFAULT_START_CAPACITY 8

typedef struct 
{
    stacktype_t *ptr;
    size_t size;
    size_t capacity;
    stacktype_t *buf;
} VStack;


VStack *vstack_create(size_t size);
void vstack_delete(VStack *vstack);

int vstack_push(VStack *vstack, stacktype_t data);
stacktype_t vstack_peek(VStack *vstack, int *rc);
stacktype_t vstack_pop(VStack *vstack, int *rc);

int vstack_display(VStack *vstack);
void vstack_extend_buf(VStack *this);

size_t get_vstack_size(size_t capacity);
bool vstack_is_empty(VStack *vstack);


#endif  // _VSTACK_H_