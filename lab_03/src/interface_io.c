#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "defines.h"
#include "utils.h"
#include "matrix.h"
#include "handler.h"

#define CHOICE_FROM 0
#define CHOICE_TO 8


void print_menu()
{
    printf(
        ANSI_COLOR_GREEN "::Работа с матрицей::\n" ANSI_COLOR_YELLOW
                         "\t1. Ввести матрицу\n"
                         "\t2. Сгенерировать случайную матрицу\n"
                         "\t3. Напечатать матрицу\n" ANSI_COLOR_GREEN 
                         "::Работа с вектором::\n" ANSI_COLOR_YELLOW
                         "\t4. Ввести вектор\n"
                         "\t5. Сгенерировать случайный вектор\n"
                         "\t6. Напечатать вектор\n" ANSI_COLOR_GREEN 
                         "::Операция умножения::\n" ANSI_COLOR_YELLOW
                         "\t7. Умножить\n"
                         "\t8. Произвести сравнение\n" ANSI_COLOR_RED
                         "\n\t0. Выход\n\n" ANSI_COLOR_RESET);
}

int input_choice(int *choice)
{
    char temp_choice[10] = {0};

    if (!fgets(temp_choice, sizeof(temp_choice), stdin)) return FAIL;
    if (temp_choice[strlen(temp_choice) - 1] != '\n') return FAIL;
    temp_choice[strlen(temp_choice) - 1] = '\0';

    if (!strlen(temp_choice))
        *choice = 0;
    else
    {
        if (!is_only_digits(temp_choice))
            return FAIL;
        if (strlen(temp_choice) > 1 && temp_choice[0] == '0')
            return FAIL;

        *choice = atoi(temp_choice);
    }

    return OK;
}

int start(int *choice)
{
    printf("Введите число в диапазоне [%d..%d] и нажмите <Enter>: ", CHOICE_FROM, CHOICE_TO);

    if (input_choice(choice) != OK)
        return ERR_INPUT_CHOICE;

    if (*choice < CHOICE_FROM || *choice > CHOICE_TO)
        return INCORRECT_CHOICE_RANGE;

    int rc = OK;
    system("clear");

    printf(ANSI_COLOR_BLUE "\nВы выбрали: " ANSI_COLOR_RED);
    switch (*choice)
    {
    case (1):
        printf("Ввести матрицу\n\n" ANSI_COLOR_RESET);
        rc = matrix_init_wrapper();
        break;

    case (2):
        printf("Сгенерировать случайную матрицу\n\n" ANSI_COLOR_RESET);
        rc = rand_matr_create_wrapper();
        break;

    case (3):
        printf("Напечатать матрицу\n\n" ANSI_COLOR_RESET);
        rc = print_matrix_wrapper();
        break;

    case (4):
        printf("Ввести вектор\n\n" ANSI_COLOR_RESET);
        rc = vector_init_wrapper();
        break;

    case (5):
        printf("Сгенерировать случайный вектор\n" ANSI_COLOR_RESET);
        rc = rand_vec_create_wrapper();
        break;

    case (6):
        printf("Напечатать вектор\n" ANSI_COLOR_RESET);
        rc = print_vector_wrapper();
        break;

    case (7):
        printf("Умножить\n" ANSI_COLOR_RESET);
        rc = mult_wrapper();
        break;

    case (8):
        printf("Произвести сравнение\n" ANSI_COLOR_RESET);
        rc = mul_analize();
        break;

    case (0):
        printf(ANSI_COLOR_RED "Выход\n" ANSI_COLOR_RESET);
        rc = exit_handler();
        break;
    }

    return rc;
}
