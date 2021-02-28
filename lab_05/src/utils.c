#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "utils.h"
#include "defines.h"


double randomDouble(const double bottom, const double top) {
    double range = top - bottom;
    double div = RAND_MAX / range;

    return bottom + (rand() / div);
}


int randomInt(const int bottom, const int top) {
    return rand() % (top - bottom + 1) + bottom;
}

int clear_input() {
    int ch = 0;

    do {
        ch = getc(stdin);
    } while ((char)ch != '\n');


    return OK;
}


void wait_and_clear() {
    printf(ANSI_COLOR_BLUE"\nНажмите <Enter>, для продолжения.\n"ANSI_COLOR_RESET);
    clear_input();
    system("clear");
}

bool is_only_digits(char *str) {
    while(*str) {
        if (!isdigit((int)(*str))) {
            return false;
        }

        str++;
    }


    return true;
}
