/**
*   \file main.c 
*   \brief файл основной программы
*   \author Ивахненко Дмитрий
*   \date 01.10.2020
*
*   Данный файл содержит основную программу.
*/

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "utils.h"
#include "car.h"
#include "cars.h"


int main(void)
{  
    int choice = -1;
    cars_t data = {0};
    keys_t key_data = {0};
    int rc = OK;
    
    while (choice)
    {
        print_start_menu();
        rc = choice_menu(&data, &key_data, &choice);

        if (rc == OK)
        {
            printf(ANSI_COLOR_GREEN"\n:::Выполнено:::\n\n"ANSI_COLOR_RESET);

            printf(ANSI_COLOR_GREEN"\n\nНажмите <Enter>, чтобы продолжить.\n"ANSI_COLOR_RESET);
            getc(stdin);
            system("clear");

            continue;
        }

        printf(ANSI_COLOR_RED"\n:::Ошибка:::\n"ANSI_COLOR_RESET);


        if (rc >= 1 && rc <= 3)
            printf(ANSI_COLOR_RED"\n:::Некорректный ввод строки:::\n\n"ANSI_COLOR_RESET);

        if (rc >= 4 && rc <= 5)
            printf(ANSI_COLOR_RED"\n:::Некорректный ввод числа:::\n\n"ANSI_COLOR_RESET);
        
        if (rc == ERR_READ_PRICE_RANGE)
            printf(ANSI_COLOR_RED"\n:::Некорректный ввод диапазона:::\n\n"ANSI_COLOR_RESET);

        if (rc == INCORRECT_CHOICE_RANGE || rc == ERR_READ_CAR_CONDITION || rc == ERR_INPUT_CHOICE)
            printf(ANSI_COLOR_RED"\n:::Такого пункта нет:::\n\n"ANSI_COLOR_RESET);

        if (rc == ERR_OVERFLOW_CARS)
            printf(ANSI_COLOR_RED"\n:::Произошло переполнение таблицы:::\n\n"ANSI_COLOR_RESET);

        if (rc == ERR_EMPTY_INPUT)
            printf(ANSI_COLOR_RED"\n:::Поле не может быть пустым:::\n\n"ANSI_COLOR_RESET);


        return rc;
    }

    return OK;
}
