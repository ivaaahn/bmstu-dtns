#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "defines.h"
#include "llstack.h"
#include "vector.h"

#define FREELIST_START_SIZE 128



LLStack *llstack_create(size_t size)
{
    LLStack *new_lls = calloc(1, sizeof(LLStack));

    new_lls->ptr = NULL;
    new_lls->curr_size = 0;
    
    new_lls->size = size;
    new_lls->freelist_ptr = vec_create(0, FREELIST_START_SIZE);
    
    return new_lls;
}


int llstack_push(LLStack *llstack, stacktype_t data)
{
    if (llstack->curr_size == llstack->size) {
        return OVERFLOW;
    }

    Node *new_node = malloc(sizeof(Node));

    new_node->data = data;
    new_node->next = llstack->ptr;
    
    llstack->ptr = new_node;
    llstack->curr_size++;

    return OK;
}


stacktype_t llstack_peek(LLStack *llstack , int *rc) 
{
    if (!(llstack->ptr)) {
        *rc = EMPTY_STACK;
        return 0;
    }

    *rc = OK;
    return llstack->ptr->data;
}


stacktype_t llstack_pop(LLStack *llstack, int *rc)
{
    if (!(llstack->ptr)) {
        *rc = UNDERFLOW;
        return 0;
    }
     
    stacktype_t data = llstack->ptr->data;
    Node *adress = llstack->ptr;

    llstack->ptr = llstack->ptr->next;
    free(adress);
    llstack->curr_size--;

    llstack->freelist_ptr = vec_push_back(llstack->freelist_ptr, (void*)adress);
    
    *rc = OK;
    return data;
}

int llstack_display(LLStack *llstack)  
{  
    if (llstack_is_empty(llstack)) {
        return EMPTY_STACK;
    }

    printf(ANSI_COLOR_YELLOW"\n[Последний элемент]<--------[Направление стека]-------->[Первый элемент]\n" ANSI_COLOR_GREEN);

    Node *curr = llstack->ptr;

    while (curr) {
        printf(ANSI_COLOR_GREEN" -> %"STYPE_SPEC"(%p)"ANSI_COLOR_RESET, curr->data, (void*)curr);
        curr = curr->next;
    }

    printf("\n");

    return OK;
}  

bool llstack_freelist_is_empty(LLStack *llstack)
{
    return vec_is_empty(llstack->freelist_ptr);
}


void list_free_all(Node *head)
{
    Node *next;
    Node *curr = head;

    while(curr) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

bool llstack_is_empty(LLStack *llstack)
{
    return !(llstack->ptr);
}

void llstack_delete(LLStack *llstack)
{
    list_free_all(llstack->ptr);
    vec_delete(llstack->freelist_ptr);
    free(llstack);
}


size_t get_llstack_size(size_t length)
{
    return sizeof(LLStack *) + sizeof(LLStack) - sizeof(Vector *) + length * sizeof(Node);
}