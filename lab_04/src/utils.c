#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "utils.h"
#include "defines.h"


int clear_input()
{
    int ch = 0;

    do {
        ch = getc(stdin);
    } while ((char)ch != '\n');


    return OK;
}


void wait_and_clear()
{
    printf(ANSI_COLOR_BLUE"\nНажмите <Enter>, для продолжения.\n"ANSI_COLOR_RESET);
    clear_input();
    system("clear");
}

bool is_only_digits(char *str)
{
    /*
        Проверяет, что строка str состоит только из цифр.
        В случае успеха, возвращает 1, иначе - 0.
    */

    while(*str) {
        if (!isdigit((int)(*str))) {
            return false;
        }

        str++;
    }


    return true;
}


bool input_stack_elem(stacktype_t *elem) {

    char temp[10] = {0};

    if (!fgets(temp, sizeof(temp), stdin)) { return false; }

    if (temp[strlen(temp) - 1] != '\n') { return false; }
    temp[strlen(temp) - 1] = '\0';

    size_t len = strlen(temp);

    if (!len || len > 1) { return false; }

    *elem = (stacktype_t)*temp;

    return true;
}