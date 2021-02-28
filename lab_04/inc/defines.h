#ifndef _DEFINES_H_
#define _DEFINES_H_

typedef char stacktype_t;
#define STYPE_SPEC "c"


#define OK 0
#define FAIL 1


#define OVERFLOW -1
#define UNDERFLOW -2
#define EMPTY_STACK -3

#define EMPTY_ADRESS_STACK -4


#define ERR_INPUT 1
#define ERR_INPUT_CHOICE 2
#define ERR_ALLOCATE 3
#define ERR_EMPTY_INPUT 4


#define ERR_CHANGE_MAX_SIZE_NOT_EMPTY_STACK 5
#define ERR_INCORRECT_NEW_STACK_SIZE 6
#define EXIT 7

#define WTH 8




/*
    bash colors
*/

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

#endif // _DEFINES_H_
