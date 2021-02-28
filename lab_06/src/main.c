#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "defines.h"
#include "io_interface.h"
#include "utils.h"



int main(void) {
    int rc = OK;

    while (true) {
        printMenu();
        if ((rc = menuHandler()) == EXIT) {
            break;
        }

        switch (rc) {
            case OK:
                break;
            
            case ERR_INPUT_CHOICE:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Некорректный ввод пункта меню::\n"ANSI_COLOR_RESET);
                break;
            
            case ERR_INPUT:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Некорректный ввод::\n"ANSI_COLOR_RESET);
                break;
            
            case ERR_FOPEN:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Файл не найден::\n"ANSI_COLOR_RESET);
                break;
            
            case ERR_FCREATE:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Не удалось создать файл::\n"ANSI_COLOR_RESET);
                break;

            default:
                printf(ANSI_COLOR_RED"\n::Неопределенная ошибка::\n"ANSI_COLOR_RESET);
                break;
        }

        wait_and_clear();
    }


    printf(ANSI_COLOR_GREEN"Приложение успешно закрыто!\n\n\n"ANSI_COLOR_RESET);

    return OK;
}