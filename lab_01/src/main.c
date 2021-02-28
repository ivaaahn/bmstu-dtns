/**
*   \file main.c 
*   \brief файл основной программы
*   \author Ивахненко Дмитрий
*   \date 21.09.2020
*
*   Данный файл содержит основную программу.
*/

#include <stdio.h>
#include <stdlib.h>

#include "parsing.h"
#include "data_structures.h"
#include "io.h"
#include "handling.h"
#include "defines.h"


int main(void)
{   
    bool_t aiszero = FALSE, biszero = FALSE;

    lrstr_t a_in;
    lreal_t a_out = {.mantissa.num = {0}, .mantissa.sign = 0, .exp = 0};

    lstr_t b_in;
    lreal_t b_out = {.mantissa.num = {0}, .mantissa.sign = 0, .exp = 0};

    lreal_t answer = {.mantissa.num = {0}, .mantissa.sign = 0, .exp = 0};

    int rc;

    welcome_print();

    if ((rc = input_real(a_in)) != OK)
    {   
        if (rc == ERR_OVERFLOW)
            printf(ANSI_COLOR_RED"\n[Ошибка] Переполнение действительного числа.\n"ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_RED"\n[Ошибка] Некорректное вещественное число.\n"ANSI_COLOR_RESET);  
        return rc;
    }

    if (input_long(b_in) != OK)
    {
        if (rc == ERR_OVERFLOW)
            printf(ANSI_COLOR_RED"\n[Ошибка] Переполнение целого числа.\n"ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_RED"\n[Ошибка] Некорректное целое число.\n"ANSI_COLOR_RESET);  
        return rc;
    }

    if ((rc = parse_real(a_in, &a_out)) != OK)
    {
        if (rc == NUMBER_IS_ZERO)
            aiszero = TRUE;
        else
        {
            if (rc == ERR_OVERFLOW)
                printf(ANSI_COLOR_RED"\n[Ошибка] Переполнение мантиссы действительного числа.\n"ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_RED"\n[Ошибка] Некорректное вещественное число.\n"ANSI_COLOR_RESET);  
            
            return rc;
        }
    }


    if ((rc = parse_integer(b_in, &b_out)) != OK)
    {
        if (rc == NUMBER_IS_ZERO)
            biszero = TRUE;
        else
            { 
                printf(ANSI_COLOR_RED"\n[Ошибка] Некорректное целое число.\n"ANSI_COLOR_RESET);
                return rc;
            }
    }

    printf(ANSI_COLOR_YELLOW "\n\nВы ввели:  "ANSI_COLOR_RESET);
    printf("%s / %s\n", a_in, b_in);

    if (biszero)
    {
        printf(ANSI_COLOR_RED"[Ошибка] Деление на ноль не допустимо.\n"ANSI_COLOR_RESET);
        return ERR_ZERO_DIVISION;
    }

    if (!aiszero)
        divide(&a_out, &b_out, &answer);
        

    if (abs(answer.exp) > 99999)
    {
        printf(ANSI_COLOR_RED"[Ошибка] Произошло переполнение экспоненты.\n"ANSI_COLOR_RESET);
        return ERR_OVERFLOW;
    }

    printf(ANSI_COLOR_GREEN "Результат вычисления: ");

    output(&answer);
    printf("\n"ANSI_COLOR_RESET);

    return OK;
}
