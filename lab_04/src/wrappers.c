#include <stdio.h>
#include <time.h>

#include "defines.h"
#include "wrappers.h"
#include "llstack.h"
#include "vstack.h"
#include "utils.h"
#include "task.h"



#define DEFAULT_TASK_STACK_SIZE 128

int push_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    int rc;
    stacktype_t data;

    printf("Введите элемент, который хотите добавить в стек (элементом является символ): ");   
 
    if (!input_stack_elem(&data)) {
        return ERR_INPUT;   
    }

    switch (stype) {
        case LLSTACK:
            rc = llstack_push(lls, data);
            break;
        
        case VSTACK:
            rc = vstack_push(vs, data);
            break;
    }


    return rc;
}

int change_size_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    switch (stype) {
        case LLSTACK: {
            printf(ANSI_COLOR_YELLOW"Текущий размер: %zu\n"ANSI_COLOR_RESET, lls->size);
            if (lls->curr_size)
                return ERR_CHANGE_MAX_SIZE_NOT_EMPTY_STACK;

            printf("Введите новый размер: ");

            long long new_max;
            if (scanf("%lld", &new_max) != 1) {
                wait_and_clear();
                return ERR_INCORRECT_NEW_STACK_SIZE;
            }

            wait_and_clear();

            if (new_max <= 0) {
                return ERR_INCORRECT_NEW_STACK_SIZE;
            }

            lls->size = (size_t)new_max;

            break;
        }
        
        case VSTACK: {
            printf(ANSI_COLOR_YELLOW"Текущий размер: %zu\n"ANSI_COLOR_RESET, vs->size);
            if (vs->ptr)
                return ERR_CHANGE_MAX_SIZE_NOT_EMPTY_STACK;

            printf("Введите новый размер: ");

            long long new_max;
            if (scanf("%lld", &new_max) != 1) {
                wait_and_clear();
                return ERR_INCORRECT_NEW_STACK_SIZE;
            }

            wait_and_clear();

            if (new_max <= 0) {
                return ERR_INCORRECT_NEW_STACK_SIZE;
            }
             
            vs->size = (size_t)new_max;

            break;
        }
    }


    return OK;
}


int peek_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    stacktype_t data;
    int rc;

    switch (stype) {
        case LLSTACK:
            data = llstack_peek(lls, &rc);
            break;
            
        case VSTACK:
            data = vstack_peek(vs, &rc);
            break;
    }

    if (rc == OK) {
        printf(ANSI_COLOR_GREEN"%"STYPE_SPEC"\n"ANSI_COLOR_RESET, data);
    }

    return rc;
}


int pop_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    stacktype_t data;
    int rc;

    switch (stype) {
        case LLSTACK:
            data = llstack_pop(lls, &rc);
            break; 
                
        case VSTACK:
            data = vstack_pop(vs, &rc);
            break;
    }

    if (rc == OK) {
        printf(ANSI_COLOR_GREEN"%"STYPE_SPEC"\n"ANSI_COLOR_RESET, data);
    }

    return rc;
}

int delete_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    int rc;

    switch (stype)
    {
        case LLSTACK:
            llstack_pop(lls, &rc);
            break; 
                
        case VSTACK:
            vstack_pop(vs, &rc);
            break;
    }

    return rc;
}

int display_wrapper(VStack *vs, LLStack *lls, stype_t stype)
{
    int rc;

    switch (stype)
    {
    case LLSTACK:
            rc = llstack_display(lls);
            break;

        case VSTACK:
            rc = vstack_display(vs);
            break;
    }


    return rc;
}
int task_wrapper(stype_t stype)
{
    int rc;
    bool res;
    char *bs = NULL;

    if (!(bs = input_bracket_sequence(&rc)))
        return rc;
    
    switch (stype) 
    {
        case LLSTACK: {
            LLStack *task_llstack = llstack_create(DEFAULT_TASK_STACK_SIZE);
            rc = is_rbs_by_lls(task_llstack, bs, &res);
            llstack_delete(task_llstack);
            break;
        }
    
        case VSTACK: {
            VStack *task_vstack = vstack_create(DEFAULT_TASK_STACK_SIZE);
            rc = is_rbs_by_vs(task_vstack, bs, &res);
            vstack_delete(task_vstack);
            break;
        }
    }

    free(bs);


    if (rc != OK) {
        return rc;
    }


    switch (res) {
        case true: {
            printf(ANSI_COLOR_GREEN"::Скобки расставлены верно::\n"ANSI_COLOR_RESET);
            break;
        }

        case false: {
            printf(ANSI_COLOR_RED"::Скобки расставлены не верно::\n"ANSI_COLOR_RESET);
            break;
        }
    }
    
    return OK;
}

int print_adresses_wrapper(LLStack *lls)
{
    
    if (llstack_freelist_is_empty(lls)) {
        return EMPTY_ADRESS_STACK;
    }


    printf(ANSI_COLOR_BLUE"Высвобожденные адреса\n"ANSI_COLOR_YELLOW);
    printf(ANSI_COLOR_YELLOW"\n[Последний элемент]<--------[Направление стека]-------->[Первый элемент]\n" ANSI_COLOR_GREEN);

    vec_rprint(lls->freelist_ptr);

    printf("\n"ANSI_COLOR_RESET);


    return OK;
}
