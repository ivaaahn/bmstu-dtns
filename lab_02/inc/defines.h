#ifndef DEFINES_H
#define DEFINES_H

#define OK 0
#define FAIL 1


/* Error return codes cars.c */

#define ERR_READ_CAR_MARQUE 1
#define ERR_READ_CAR_COUNTRY 2
#define ERR_READ_CAR_COLOR 3

#define ERR_READ_CAR_PRICE 4
#define ERR_READ_CAR_COND_ATTR 5

#define ERR_READ_CAR_CONDITION 7

#define ERR_READ_PRICE_RANGE 8

#define ERR_OVERFLOW_CARS 9


/* Error return codes utils.c */

#define ERR_INPUT_CHOICE -1
#define INCORRECT_CHOICE_RANGE -2



/* Error return codes car.c */

#define ERR_EMPTY_INPUT 11



/*
    bash colors
*/

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

#endif // DEFINES_H
