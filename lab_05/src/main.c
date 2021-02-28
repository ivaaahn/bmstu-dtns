#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "defines.h"
#include "queue.h"
#include "request.h"
#include "io_interface.h"
#include "utils.h"

int main(void) {
    srand(clock());
    timeConstReset();
    Queue *queue = queueCreateAll();
    int rc = OK;


    while (true) {
        printMenu();

        if ((rc = menuHandler(queue)) == EXIT) {
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

            case OVERFLOW:
                printf(ANSI_COLOR_RED"\n[Ошибка] ::Очередь переполнена::\n"ANSI_COLOR_RESET);
                break;
            
            default:
                printf(ANSI_COLOR_RED"\n[Критическая Ошибка] ::Неопределенная ошибка::\n"ANSI_COLOR_RESET);
                break;
        }

        wait_and_clear();
    }


    printf(ANSI_COLOR_GREEN"Приложение успешно закрыто!\n\n\n"ANSI_COLOR_RESET);
    queueDeleteAll(queue);

    return OK;
}