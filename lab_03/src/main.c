#include <stdlib.h>
#include <stdio.h>

// #include "handler.h"
#include "utils.h"
#include "defines.h"
#include "interface_io.h"



int main() 
{
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);


    int choice = -1;
    int rc = OK;

    while (choice)
    {
        print_menu();
        rc = start(&choice);

        if (rc == OK)
        {
            wait_and_clear();
            continue;
        }

        if (rc == INCORRECT_CHOICE_RANGE || rc == ERR_INPUT_CHOICE)
            printf(ANSI_COLOR_RED"\n::Некорректный ввод пункта меню::\n"ANSI_COLOR_RESET);
        else if (rc == ERR_INPUT)
            printf(ANSI_COLOR_RED"\n::Некорректный ввод::\n"ANSI_COLOR_RESET);
        
    
        printf(ANSI_COLOR_RED":::Ошибка:::\n"ANSI_COLOR_RESET);
        wait_and_clear();
        continue;
    }


    return OK;
}