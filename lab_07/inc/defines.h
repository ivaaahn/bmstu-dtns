#ifndef _DEFINES_H_
#define _DEFINES_H_

#define INF INT_MAX
#define NONE INT_MIN

#define OK 0
#define FAIL 1

#define CALC_TYPE CLOCK_MONOTONIC
#define DELTA (1e9 * (toc.tv_sec - tic.tv_sec) + (toc.tv_nsec - tic.tv_nsec))

#define T_INIT struct timespec tic, toc
#define T_BEGIN (clock_gettime(CALC_TYPE, &tic))
#define T_END (clock_gettime(CALC_TYPE, &toc))


/*
    bash colors
*/

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define ERR_LABEL ANSI_COLOR_RED "[Ошибка] " ANSI_COLOR_RESET

#define ERR_LAUNCH 1
#define ERR_LAUNCH_VERB                                         \
    {                                                           \
        printf(ERR_LABEL "Вы ввели недостаточно аргументов\n"); \
        return ERR_LAUNCH;                                      \
    }

#define ERR_FOPEN 2
#define ERR_FOPEN_VERB                                 \
    {                                                  \
        printf(ERR_LABEL "Не удалось открыть файл\n"); \
        return ERR_FOPEN;                              \
    }

#define ERR_INPUT 3
#define ERR_INPUT_VERB                           \
    {                                            \
        printf(ERR_LABEL "Некорректный ввод\n"); \
        return ERR_INPUT;                        \
    }


#endif // _DEFINES_H_
