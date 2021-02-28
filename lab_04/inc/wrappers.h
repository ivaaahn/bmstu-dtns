#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <stdbool.h>

#include "vstack.h"
#include "llstack.h"
#include "utils.h"

typedef enum {
    LLSTACK,
    VSTACK
} stype_t;


int push_wrapper(VStack *vs, LLStack *lls, stype_t stype);
int peek_wrapper(VStack *vs, LLStack *lls, stype_t stype);
int pop_wrapper(VStack *vs, LLStack *lls, stype_t stype);
int delete_wrapper(VStack *vs, LLStack *lls, stype_t stype);
int display_wrapper(VStack *vs, LLStack *lls, stype_t stype);
int task_wrapper(stype_t stype);
int print_adresses_wrapper(LLStack *lls);
int change_size_wrapper(VStack *vs, LLStack *lls, stype_t stype);



#endif  // _WRAPPERS_H_