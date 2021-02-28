#ifndef DEFINES_H
#define DEFINES_H


/*
    some errors exceptions
*/

#define ERR_OVERFLOW 2
#define ERR_MANTISSA_OVERFLOW 3
#define ERR_EXP_OVERFLOW 4 
#define ERR_SIGN_PARSE 3
#define ERR_DOT_PARSE 4
#define NOSIGN 5
#define ERR_INTEGER_PARSE 6
#define ERR_S_DIGIT_PARSE 7
#define ERR_REAL_PARSE 8
#define ERR_EXP_PARSE 9

#define NUMBER_IS_ZERO 10

#define ERR_INPUT 11
#define ERR_ZERO_DIVISION 12


/*
    data_structures.c defines
*/

#define MANT_LEN 30
#define EXP_LEN 5

#define LEN_INPUT_LONG 33
#define LEN_INPUT_LREAL 41


/*
    Macroreplacements
*/


#define OK 0
#define FAIL 1

#define POSITIVE 1
#define NEGATIVE -1

/*
    bash colors
*/

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#endif // DEFINES_H
