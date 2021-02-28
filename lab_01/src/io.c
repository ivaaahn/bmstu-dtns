#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "io.h"
#include "data_structures.h"
#include "defines.h"


int input_real(lrstr_t str)
{
     printf(ANSI_COLOR_GREEN
              "                                                       0        10        20        30       MAX\n"
              "                                                       |123456789|123456789|123456789|12345678|\n"ANSI_COLOR_RESET
              "Введите делимое (вещественное число) и нажмите 'Enter': ");

    if (!fgets(str, LEN_INPUT_LREAL, stdin))
        return ERR_INPUT;
    
    if (str[strlen(str) - 1] != '\n')
        return ERR_OVERFLOW;

    str[strlen(str) - 1] = '\0';
   
    return OK;
}

int input_long(lstr_t str)
{
     printf(ANSI_COLOR_GREEN
              "\n\n                                                       0        10        20        30 MAX\n"
              "                                                       |123456789|123456789|123456789||\n"ANSI_COLOR_RESET
              "Введите делитель (целое число) и нажмите 'Enter':       ");

    if (!fgets(str, LEN_INPUT_LONG, stdin))
        return ERR_INPUT;
    
    int8_t len = strlen(str);

    if (len < 2)
        return ERR_INPUT;

    if (len == 2 && !isdigit((int)str[0]))
        return ERR_INPUT;
    
    if (str[len - 1] != '\n')
        return ERR_OVERFLOW;

    str[len-- - 1] = '\0';

    if (len == LEN_INPUT_LONG - 2 && (isdigit((int)str[0])))
        return ERR_OVERFLOW;

    return OK;
}  


void output(lreal_t *num)
{
    int8_t last_sign_digit = MANT_LEN;
    
    while (last_sign_digit >= 0 && !num->mantissa.num[last_sign_digit])
        last_sign_digit--;

    if (num->mantissa.sign < 0)
        printf("-");

    printf("%d.", num->mantissa.num[0]);
    
    for (int i = 1; i <= last_sign_digit; i++)
        printf("%d", num->mantissa.num[i]);
    
    printf("E%d", num->exp);
}

void welcome_print()
{
       printf(ANSI_COLOR_GREEN
              "\nВЫЧИСЛЕНИЕ ЧАСТНОГО ВЕЩЕСТВЕННОГО И ЦЕЛОГО ЧИСЕЛ\n\n"
              ANSI_COLOR_RED"Правила ввода:\n" ANSI_COLOR_YELLOW
              "1. При вводе целого числа максимальная длина составляет 31 символ:\n"
              " - 1 символ на знак числа (+/-) (опционально)\n"
              " - до 30 символов на само число (0-9).\n"
              "2. При вводе вещественного числа максимальная длина составляет 39 символов:\n"
              " - 1 символ на знак числа (+/-) (опционально),\n"
              " - 1 символ на точку (.) (опционально)\n"
              " - до 30 символов на мантиссу (0-9)\n"
              " - 1 символ на знак экспоненты (e/E) (опционально)\n"
              " - 1 символ на знак порядка (+/-) (опционально)\n"
              " - до 5 символов на порядок (0-9).\n"
              "3. Ведущие нули допустимы.\n"
              "4. Пробелы в числах недопустимы\n"
              "5. Вещественная точка может записываться ТОЛЬКО в виде точки (запятая ',' недопустима).\n"
              "6. Символ экспоненты не чувствителен к регистру (допустимо исопользовать и 'e', и 'E).'\n\n"ANSI_COLOR_RESET);
}