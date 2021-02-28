#ifndef _IO_INTERFACE_H_
#define _IO_INTERFACE_H_

#include "queue.h"

typedef enum {
    CH_EXIT = 0,
    CH_START_ARR = 1,
    CH_START_LIST = 2,
    CH_CHANGE_DELAY = 3,
    CH_CHANGE_HANDL = 4,
    CH_CHANGE_PROBAB = 5,
    CH_CHANGE_TARGET = 6,
    CH_RES_VALUES = 7,
    CH_PRINT_CONST = 8,
    CH_ANALYZE = 9,
    CH_PRINT_ADRR = 10,
    CH_PRINT_ERR = 11,
    CH_COUNT,
    CH_ERR
} choice_t;


int menuHandler(Queue *queue);
void printMenu();


#endif  // _IO_INTERFACE_H_