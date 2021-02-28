#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "defines.h"

// #include "matrix.h"
// #include "handler.h"



int clear_input()
{
    int ch = 0;

    do
    {
        ch = getc(stdin);
    } while ((char)ch != '\n');


    return OK;
}


int wait_and_clear()
{
    printf("Нажмите <Enter>, для продолжения.\n");
    clear_input();
    system("clear");


    return OK;
}

int is_only_digits(char *str)
{
    /*
        Проверяет, что строка str состоит только из цифр.
        В случае успеха, возвращает 1, иначе - 0.
    */

    size_t i = 0;

    while(str[i])
    {
        if (!isdigit((int)str[i]))
            return 0;
        i++;
    }
    return 1;
}

