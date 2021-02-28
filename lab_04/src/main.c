#include <stdio.h>
#include <stdbool.h>

#include "defines.h"
#include "llstack.h"
#include "vstack.h"
#include "handling.h"

#define DEFAULT_STACK_SIZE 64

int main(void)
{
    int rc = OK;

    LLStack *llstack = llstack_create(DEFAULT_STACK_SIZE);
    VStack *vstack = vstack_create(DEFAULT_STACK_SIZE);

    if (!vstack || !llstack) {
        printf(ANSI_COLOR_RED"\n[Критическая Ошибка] ::Не удалось получить доступ к стеку::\n"ANSI_COLOR_RESET);
        return ERR_ALLOCATE;
    }


    while (true) 
    {
        print_menu();

        if ((rc = main_handler(vstack, llstack)) == EXIT)
            break;

        switch (rc) {
            case OK:
                break;
            
            case ERR_INPUT_CHOICE:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Некорректный ввод пункта меню::\n"ANSI_COLOR_RESET);
                break;
            
            case ERR_INPUT:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Некорректный ввод::\n"ANSI_COLOR_RESET);
                break;

            case OVERFLOW:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Стек переполнен (Stack Overflow)::\n"ANSI_COLOR_RESET);
                break;

            case UNDERFLOW:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::В стеке нет ни одного элемента (Stack Underflow)::\n"ANSI_COLOR_RESET);
                break;

            case EMPTY_STACK:
                printf(ANSI_COLOR_RED"\n[Предупреждение] ::Стек пуст::\n"ANSI_COLOR_RESET);
                break;

            case EMPTY_ADRESS_STACK:
                printf(ANSI_COLOR_RED"\n[Предупреждение] ::Адреса еще не высвобождались::\n"ANSI_COLOR_RESET);
                break;
            
            case ERR_EMPTY_INPUT:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Последовательность не может быть пустой::\n"ANSI_COLOR_RESET);
                break;

            case ERR_CHANGE_MAX_SIZE_NOT_EMPTY_STACK:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Изменение размера возможно только для пустого стека::\n"ANSI_COLOR_RESET);
                break;

            case ERR_INCORRECT_NEW_STACK_SIZE:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Некорректный размер стека::\n"ANSI_COLOR_RESET);
                break;

            default:
                printf(ANSI_COLOR_RED"\n[Критическая Ошибка] ::Неопределенная ошибка::\n"ANSI_COLOR_RESET);
                break;
        }

        wait_and_clear();
    }


    llstack_delete(llstack);
    vstack_delete(vstack);
    printf(ANSI_COLOR_GREEN"Приложение успешно закрыто!\n\n\n"ANSI_COLOR_RESET);

    return OK;
}