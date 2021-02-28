#ifndef _DEFINES_H_
#define _DEFINES_H_


#define OK 0
#define FAIL 1

#define DIGTOCHR(digit) ('0' + digit)
#define EMPTY (INT_MIN + 1)
#define REMOVED (INT_MAX - 1)
#define EMPTY_PRINT "FREE"
#define REMOVED_PRINT "REMOVED"

typedef int treeType_t;
#define treeSpec "d"

typedef int htItem_t;
#define htItemSpec "d"


// #define CALC_TYPE CLOCK_THREAD_CPUTIME_ID
// #define CALC_TYPE CLOCK_REALTIME
#define CALC_TYPE CLOCK_MONOTONIC
#define DELTA (1e9 * (toc.tv_sec - tic.tv_sec) + (toc.tv_nsec - tic.tv_nsec))

#define T_INIT struct timespec tic, toc
#define T_BEGIN (clock_gettime(CALC_TYPE, &tic))
#define T_END (clock_gettime(CALC_TYPE, &toc))


#define OVERFLOW -1
#define UNDERFLOW -2

#define ERR_INPUT 1
#define ERR_INPUT_CHOICE 2
#define ERR_ALLOCATE 3
#define ERR_EMPTY_INPUT 4
#define ERR_FOPEN 11
#define HT_OVERFLOW 12
#define ERR_FCREATE 13
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
