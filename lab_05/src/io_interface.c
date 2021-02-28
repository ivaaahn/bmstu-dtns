#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "io_interface.h"
#include "defines.h"
#include "queue.h"
#include "handling.h"
#include "print_statistics.h"


static choice_t getChoice() {
    char tmpChoice[64] = { 0 };
    printf("Введите целое число в диапазоне [%d..%d] и нажмите <Enter>: ", CH_EXIT, CH_COUNT - 1);
    if (!fgets(tmpChoice, sizeof(tmpChoice), stdin)) { 
        return CH_ERR; 
    }

    if (!strncmp(tmpChoice, "1\n", 2)) {
        return 1;
    } else if (!strncmp(tmpChoice, "2\n", 2)) {
        return 2;
    } else if (!strncmp(tmpChoice, "3\n", 2)) {
        return 3;
    } else if (!strncmp(tmpChoice, "4\n", 2)) {
        return 4;
    } else if (!strncmp(tmpChoice, "5\n", 2)) {
        return 5;
    } else if (!strncmp(tmpChoice, "6\n", 2)) {
        return 6;
    } else if (!strncmp(tmpChoice, "7\n", 2)) {
        return 7;
    } else if (!strncmp(tmpChoice, "8\n", 2)) {
        return 8;
    } else if (!strncmp(tmpChoice, "9\n", 2)) {
        return 9;
    } else if (!strncmp(tmpChoice, "0\n", 2)) {
        return 0;
    } else if (!strncmp(tmpChoice, "10\n", 3)) {
        return 10;
    } else if (!strncmp(tmpChoice, "11\n", 3)) {
        return 11;
    } else {
        return CH_ERR;
    }
}


int menuHandler(Queue *queue) {
    choice_t choice = getChoice();
    system("clear");
    int rc;
    switch (choice) {
        case CH_START_ARR:
            rc = modeling(queue, Q_ARRAY, false, NULL);
            if (rc == OK && isPrintErr()) {
                calcErrorWrapper(queue, Q_ARRAY);
            }
            return rc;
        case CH_START_LIST:
            rc = modeling(queue, Q_LIST, false, NULL);
            if (rc == OK && isPrintErr()) {
                calcErrorWrapper(queue, Q_LIST);
            }
            return rc;
        case CH_CHANGE_DELAY:
            return setDelayInterval();
        case CH_CHANGE_HANDL:
            return setHandlingInterval();
        case CH_RES_VALUES:
            return timeConstReset();
        case CH_ANALYZE:
            return analyze(queue);
        case CH_PRINT_ADRR:
            return changeFlagPrintAddr();
        case CH_CHANGE_PROBAB:
            return setProbability();
        case CH_CHANGE_TARGET:
            rc = setTarget();
            if (rc == OK) {
                queueDelete(queue, Q_ARRAY);
                queueCreate(queue, Q_ARRAY);
            }
            return rc;
        case CH_PRINT_CONST:
            return printAllConst();
        case CH_PRINT_ERR:
            return changeFlagPrintErr();
        case CH_EXIT:
            changeFlagPrintAddr();
            return EXIT;
        case CH_ERR:
            return ERR_INPUT_CHOICE;
        default:
            return WTH;
    }
}

void printMenu() {
    printf(
        ANSI_COLOR_RED 
        "\n\n\t======:::МЕНЮ:::======\n"
        ANSI_COLOR_YELLOW
        "\t1. Смоделировать очередь на массиве\n"
        "\t2. Смоделировать очередь на списке\n"
        "\t3. Изменить интервал времени прихода \n"
        "\t4. Изменить инетрвал времени обработки\n"
        "\t5. Изменить вероятность ухода заявки на второй круг\n"
        "\t6. Изменить необходимое кол-во закрытых заявок\n"
        "\t7. Сбросить значения констант до стандартных\n"
        "\t8. Просмотреть значения констант\n"
        "\t9. Провести анализ\n");
    
    if (isPrintAddr()) {
        printf(
            ANSI_COLOR_GREEN
            "\t10. Печать адресов (включено)\n"
            ANSI_COLOR_RESET);
    } else {
        printf(
            ANSI_COLOR_RED
            "\t10. Печать адресов (выключено)\n"
            ANSI_COLOR_RESET);
    }

    if (isPrintErr()) {
        printf(
            ANSI_COLOR_GREEN
            "\t11. Подсчет погрешности (включено)\n"
            ANSI_COLOR_RESET);
    } else {
        printf(
            ANSI_COLOR_RED
            "\t11. Подсчет погрешности (выключено)\n"
            ANSI_COLOR_RESET);
    }


    printf(
            ANSI_COLOR_RED
            "\n\t0. Выход\n\n"
            ANSI_COLOR_RESET);
}