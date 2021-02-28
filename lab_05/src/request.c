#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "request.h"


TimeConstants timeConstants;

int timeConstReset() {
    timeConstants.delayMin = 0.0;
    timeConstants.delayMax = 6.0;
    timeConstants.handlTimeMin = 0.0;
    timeConstants.handlTimeMax = 1.0;
    timeConstants.probability = 0.8;
    timeConstants.targetAmount = 1000;

    return OK;
}

int setDelayInterval() {
    printf("Текущий интервал: "ANSI_COLOR_GREEN"[%.3lf, %.3lf]\n"ANSI_COLOR_RESET, \
        timeConstants.delayMin, timeConstants.delayMax);

    printf(ANSI_COLOR_YELLOW"Введите границы интервала"
    " (два действительных числа через пробел): "ANSI_COLOR_RESET);

    
    double tmpMin, tmpMax;
    int rc = (scanf("%lf%lf", &tmpMin, &tmpMax) == 2);
    wait_and_clear();


    rc = rc ? (0.0 <= tmpMin && tmpMin <= tmpMax) : rc;

    if (rc) {
        timeConstants.delayMin = tmpMin;
        timeConstants.delayMax = tmpMax;
    }

    return rc ? OK : ERR_INPUT;
}


int setProbability() {
    printf("Текущая вероятность " ANSI_COLOR_GREEN"P = %.3lf\n"ANSI_COLOR_RESET, \
        timeConstants.probability);

    printf(ANSI_COLOR_YELLOW"Введите значение вероятности: "ANSI_COLOR_RESET);

    double tmp;
    int rc = (scanf("%lf", &tmp) == 1);
    wait_and_clear();

    rc = rc ? (0.0 <= tmp && tmp <= 1.0) : rc;

    if (rc) {
        timeConstants.probability = tmp;
    }

    return rc ? OK : ERR_INPUT;
}

int setTarget() {
    printf("Текущее кол-во необходимых заявок: "ANSI_COLOR_GREEN"%zu\n"ANSI_COLOR_RESET, \
        timeConstants.targetAmount);

    printf(ANSI_COLOR_YELLOW"Введите новое количество: "ANSI_COLOR_RESET);

    long long tmp;
    int rc = (scanf("%lld", &tmp) == 1);
    wait_and_clear();

    rc = rc ? (0 < tmp) : rc;

    if (rc) {
        timeConstants.targetAmount = (size_t)tmp;
    }

    return rc ? OK : ERR_INPUT;
}

int printAllConst() {
    printf("Текущий интервал прихода: "ANSI_COLOR_GREEN"[%.3lf, %.3lf]\n"ANSI_COLOR_RESET, \
        timeConstants.delayMin, timeConstants.delayMax);

    printf("Текущий интервал обработки: "ANSI_COLOR_GREEN"[%.3lf, %.3lf]\n"ANSI_COLOR_RESET, \
        timeConstants.handlTimeMin, timeConstants.handlTimeMax);

    printf("Текущая вероятность " ANSI_COLOR_GREEN"P = %.3lf\n"ANSI_COLOR_RESET, \
        timeConstants.probability);

    printf("Текущее кол-во необходимых заявок: "ANSI_COLOR_GREEN"%zu\n"ANSI_COLOR_RESET, \
        timeConstants.targetAmount);

    return OK;
}

int setHandlingInterval() {
    printf("Текущий интервал: "ANSI_COLOR_GREEN"[%.3lf, %.3lf]\n"ANSI_COLOR_YELLOW, \
        timeConstants.handlTimeMin, timeConstants.handlTimeMax);

    printf("Введите границы интервала (два действительных числа через пробел): "ANSI_COLOR_RESET);

    double tmpMin, tmpMax;
    int rc = (scanf("%lf%lf", &tmpMin, &tmpMax) == 2);
    wait_and_clear();

    rc = rc ? (0.0 <= tmpMin && tmpMin <= tmpMax) : rc;

    if (rc) {
        timeConstants.handlTimeMin = tmpMin;
        timeConstants.handlTimeMax = tmpMax;
    }

    return rc ? OK : ERR_INPUT;
}

void requestGenerate(Request *request, double currTime) {
    request->handlT = randomDouble(timeConstants.handlTimeMin, timeConstants.handlTimeMax);
    request->deliverT = currTime + randomDouble(timeConstants.delayMin, timeConstants.delayMax);
}

void requestReGenerate(Request *request, double currTime) {
    request->handlT = randomDouble(timeConstants.handlTimeMin, timeConstants.handlTimeMax);
    request->deliverT = currTime;
}

