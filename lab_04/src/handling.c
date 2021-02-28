#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


#include "handling.h"
#include "wrappers.h"
#include "defines.h"
#include "utils.h"
#include "task.h"


void print_menu()
{
    printf(
        ANSI_COLOR_RED "\n\n======:::МЕНЮ:::======\n"
        ANSI_COLOR_BLUE ":::Работа со стеком:::\n" ANSI_COLOR_GREEN
                         "::Стек на односвязном списке::\n"ANSI_COLOR_YELLOW
                         "\t1. Добавить элемент\n"
                         "\t2. Просмотреть последний элемент\n"
                         "\t3. Извлечь последний элемент\n"
                         "\t4. Удалить последний элемент (без вывода)\n"
                         "\t5. Просмотреть сожержимое стека\n"
                         "\t6. Изменить размер стека\n" ANSI_COLOR_GREEN 
                         "::Стек на векторе::\n" ANSI_COLOR_YELLOW
                         "\t7. Добавить элемент\n"
                         "\t8. Просмотреть последний элемент\n"
                         "\t9. Извлечь последний элемент\n"
                         "\t10. Удалить последний элемент (без вывода)\n"
                         "\t11. Просмотреть содержимое стека\n" 
                         "\t12. Изменить размер стека\n" ANSI_COLOR_BLUE 
                         ":::Задача на правильную скобочную последовательность:::\n" ANSI_COLOR_YELLOW
                         "\t13. Проверить расстановку скобок в строке (Стек-Список)\n"
                         "\t14. Проверить расстановку скобок в строке (Стек-Вектор)\n"ANSI_COLOR_BLUE
                         ":::Дополнительные функции:::\n" ANSI_COLOR_YELLOW
                         "\t15. Просмотреть список высвобожденных адресов\n"
                         "\t16. Провести сравнительный анализ (Стек-Ветор и Стек-Список)\n"ANSI_COLOR_RED
                         "\n\t0. Выход\n\n" ANSI_COLOR_RESET);
}

int input_choice(int *choice)
{
    char temp_choice[10] = {0};

    if (!fgets(temp_choice, sizeof(temp_choice), stdin)) { return FAIL; }
    if (temp_choice[strlen(temp_choice) - 1] != '\n') { return FAIL; }
    temp_choice[strlen(temp_choice) - 1] = '\0';

    if (!strlen(temp_choice)) {
        *choice = 0;
    }
    else {
        if (!is_only_digits(temp_choice)) { 
            return FAIL;
        }

        if (strlen(temp_choice) > 1 && temp_choice[0] == '0') {
            return FAIL;
        }

        *choice = atoi(temp_choice);
    }

    return OK;
}

int main_handler(VStack *vstack, LLStack *llstack)
{
    int choice;

    printf("Введите целое число в диапазоне [%d..%d] и нажмите <Enter>: ", CHOICE_FROM, CHOICE_TO);

    if (input_choice(&choice) != OK) {
        return ERR_INPUT_CHOICE;
    }

    if (choice < CHOICE_FROM || choice > CHOICE_TO) {
        return ERR_INPUT_CHOICE;
    }

    int rc = OK;
    system("clear");

    switch (choice)
    {
        case (1):
            rc = push_wrapper(vstack, llstack, LLSTACK);
            break;

        case (2):
            rc = peek_wrapper(vstack, llstack, LLSTACK);
            break;

        case (3):
            rc = pop_wrapper(vstack, llstack, LLSTACK);
            break;

        case (4):
            rc = delete_wrapper(vstack, llstack, LLSTACK);
            break;

        case (5):
            rc = display_wrapper(vstack, llstack, LLSTACK);
            break;

        case (6):
            rc = change_size_wrapper(vstack, llstack, LLSTACK);
            break;

        case (7):
            rc = push_wrapper(vstack, llstack, VSTACK);
            break;

        case (8):
            rc = peek_wrapper(vstack, llstack, VSTACK);
            break;

        case (9):
            rc = pop_wrapper(vstack, llstack, VSTACK);
            break;

        case (10):
            rc = delete_wrapper(vstack, llstack, VSTACK);
            break;

        case (11):
            rc = display_wrapper(vstack, llstack, VSTACK);
            break;

        case (12):
            rc = change_size_wrapper(vstack, llstack, VSTACK);
            break;

        case (13):
            rc = task_wrapper(LLSTACK);
            break;

        case (14):
            rc = task_wrapper(VSTACK);
            break;

        case (15):
            rc = print_adresses_wrapper(llstack);
            break;
        
        case (16):
            rc = analyze();
            break;

        case (0):
            rc = EXIT;
            break;

        default:
            rc = WTH;
            break;
    }


    return rc;
}


int analyze()
{
    int rc = OK;
    clock_t start;

    const size_t repeats = 1000;


    printf(ANSI_COLOR_BLUE"#### Сравнительный анализ стека, построенного на векторе и односвязном списке ####\n"ANSI_COLOR_YELLOW);
    printf("Для каждого измерения взято среднее значение\n");
    printf("по времени для %4zu повторений", repeats);
    printf("\nОбъём памяти указан в байтах.\n\n"ANSI_COLOR_RESET);

    printf("|  Длина   |Время добавления (µs)| Время удаления (µs) |  Объём стека (байт) |\n");
    printf("|  стека   |  вектор  |  список  |  вектор  |  список  |  вектор  |  список  |\n");
    printf("|----------|----------|----------|----------|----------|----------|----------|\n");


    for (size_t length = 32, length2 = length + 1; length < 1000000; length *= 2, length2 = length + 1)
    {
        double fulltime_vs_push = 0, fulltime_lls_push = 0;
        double fulltime_vs_pop = 0, fulltime_lls_pop = 0;
        
        size_t capacity;

        for (size_t rep = 0; rep < repeats; rep++) {
            VStack *vstack = vstack_create((size_t)-1);
            LLStack *llstack = llstack_create((size_t)-1);

            start = clock();
            for (size_t i = 0; i < length; i++) {
                vstack_push(vstack, 'a');
            }
            fulltime_vs_push += clock() - start;

            capacity = vstack->capacity;

            start = clock();
            for (size_t i = 0; i < length; i++) {
                llstack_push(llstack, 'a');
            }
            fulltime_lls_push += clock() - start;


            start = clock();
            for (size_t i = 0; i < length; i++) {
                vstack_pop(vstack, &rc);
            }
            fulltime_vs_pop += clock() - start;

            start = clock();
            for (size_t i = 0; i < length; i++) {
                llstack_pop(llstack, &rc);
            }
            fulltime_lls_pop += clock() - start;

            llstack_delete(llstack);
            vstack_delete(vstack);
        }

        printf("|%10zu|%10.3lf|%10.3lf|%10.3lf|%10.3lf|%10zu|%10zu|\n", 
                length, 
                fulltime_vs_push * 1000000 / CLOCKS_PER_SEC / repeats, 
                fulltime_lls_push * 1000000 / CLOCKS_PER_SEC / repeats, 
                fulltime_vs_pop * 1000000 / CLOCKS_PER_SEC / repeats,
                fulltime_lls_pop * 1000000 / CLOCKS_PER_SEC / repeats,
                get_vstack_size(capacity),
                get_llstack_size(length)); 

        fulltime_vs_push = 0; 
        fulltime_lls_push = 0;
        fulltime_vs_pop = 0;
        fulltime_lls_pop = 0;


        for (size_t rep = 0; rep < repeats; rep++) {
            VStack *vstack = vstack_create((size_t)-1);
            LLStack *llstack = llstack_create((size_t)-1);

            start = clock();
            for (size_t i = 0; i < length2; i++) {
                vstack_push(vstack, 'a');
            }
            fulltime_vs_push += clock() - start;
            
            capacity = vstack->capacity;

            start = clock();
            for (size_t i = 0; i < length2; i++) {
                llstack_push(llstack, 'a');
            }
            fulltime_lls_push += clock() - start;


            start = clock();
            for (size_t i = 0; i < length2; i++) {
                vstack_pop(vstack, &rc);
            }
            fulltime_vs_pop += clock() - start;

            start = clock();
            for (size_t i = 0; i < length2; i++) {
                llstack_pop(llstack, &rc);
            }
            fulltime_lls_pop += clock() - start;

            llstack_delete(llstack);
            vstack_delete(vstack);
        }

        printf("|%10zu|%10.3lf|%10.3lf|%10.3lf|%10.3lf|%10zu|%10zu|\n", 
                length2, 
                fulltime_vs_push * 1000000 / CLOCKS_PER_SEC / repeats, 
                fulltime_lls_push * 1000000 / CLOCKS_PER_SEC / repeats, 
                fulltime_vs_pop * 1000000 / CLOCKS_PER_SEC / repeats,
                fulltime_lls_pop * 1000000 / CLOCKS_PER_SEC / repeats,
                get_vstack_size(capacity),
                get_llstack_size(length2));
    }

    return OK;
}

