#ifndef _LLSTACK_H_
#define _LLSTACK_H_

#include "vstack.h"
#include "vector.h"


typedef struct Node Node;

// #pragma pack(push, 1)
struct Node {
    stacktype_t data;
    Node *next;
};
// #pragma pack(pop)


typedef struct {
    Node *ptr;
    size_t size;
    size_t curr_size;
    Vector *freelist_ptr;
} LLStack;


LLStack *llstack_create(size_t size);
void list_free_all(Node *head);
void llstack_delete(LLStack *llstack);


int llstack_push(LLStack *llstack, stacktype_t data);
stacktype_t llstack_pop(LLStack *llstack, int *rc);
stacktype_t llstack_peek(LLStack *llstack , int *rc);

int llstack_display(LLStack *llstack);

bool llstack_is_empty(LLStack *llstack);
bool llstack_freelist_is_empty(LLStack *llstack);

size_t get_llstack_size(size_t length);


#endif  // _LLSTACK_H__