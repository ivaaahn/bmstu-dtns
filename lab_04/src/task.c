#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "defines.h"
#include "vstack.h"
#include "llstack.h"
#include "task.h"
#include "utils.h"

const char *checker = "{}()[]";
const char *left = "([{";
const char *right = ")]}";

// figure
#define LF '{'
#define RF '}'

// ordinary
#define LO '('
#define RO ')'

// square
#define LS '['
#define RS ']'


char *input_bracket_sequence(int *rc)
{
    char *bs = NULL;
    size_t n = 0;

    printf("Введите строку, которую необходимо проверить: ");
    if (getline(&bs, &n, stdin) == -1) {
        free(bs);
        *rc = ERR_INPUT;
        return NULL;
    }

    bs[strlen(bs) - 1] = '\0';

    if (!strlen(bs)) {
        *rc = ERR_EMPTY_INPUT;
        return NULL;
    }

    return bs;
}


//  is right bracket sequence
int is_rbs_by_lls(LLStack *llstack, const char *str, bool *res)
{
    int rc;
    stacktype_t last;
    char curr;

    *res = false;
    for ( ; (curr = *str); str++)
    {
        if (strchr(left, curr)) {
            rc = llstack_push(llstack, curr);
            if (rc != OK)  // left bracket should be pushed 
                return rc;
        }
        else if (strchr(right, curr)) {
            if (llstack_is_empty(llstack)) {  // extra right bracket
                return OK;
            }

            last = llstack_pop(llstack, &rc);
            // last = pop_lls(llstack, &rc, &time);  // check last bracket in stack
            if (rc != OK) {
                return rc;
            }


            if (last == LF && curr == RF) {
                continue;
            }
            
            if (last == LO && curr == RO) {
                continue;
            }

            if (last == LS && curr == RS) {
                continue;
            }

            return OK;
        }   
        
    }

    if (llstack_is_empty(llstack)) {
        *res = true;
    }

    
    return OK;
}


int is_rbs_by_vs(VStack *vstack, const char *str, bool *res)
{
    int rc;
    stacktype_t last;
    char curr;

    *res = false;
    for ( ; (curr = *str); str++)
    {
        if (strchr(left, curr)) {
            rc = vstack_push(vstack, curr);
            if (rc != OK) {  // left bracket should be pushed 
                return rc;
            }
        }
        else if (strchr(right, curr)) {
            if (vstack_is_empty(vstack)) { // extra right bracket
                return OK;
            }

            last = vstack_pop(vstack, &rc);  // check last bracket in stack
            if (rc != OK) {
                return rc;
            }


            if (last == LF && curr == RF) {
                continue;
            }
            
            if (last == LO && curr == RO) {
                continue;
            }

            if (last == LS && curr == RS) {
                continue;
            }

            return OK;
        }
    }

    if (vstack_is_empty(vstack)) {
        *res = true;
    }


    return OK;
}