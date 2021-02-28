#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "request.h"
#include "utils.h"
#include "defines.h"


static double relativeError(double a, double b) {
    double min = a < b ? a : b;
    double max = a > b ? a : b;

    return (max - min) / min * 100;
}


void printStatAll(const double modelT, const double waitT, const size_t numOfServ,\
                            const size_t closedReq, const double sumTimeInQueue, const size_t openedReq) {
    printf(
        ANSI_COLOR_GREEN
        "\n=========================================================\n"
        "Время моделирования:\t\t\t%.3lf е.в.\n"
        "Кол-во срабатываний ОА:\t\t\t%zu\n"
        "Время работы:\t\t\t\t%.3lf е.в. (~%.2lf%%)\n"
        "Время простоя:\t\t\t\t%.3lf е.в. (~%.2lf%%) \n"
        "Кол-во вошедших в систему заявок:\t%zu\n"
        "Кол-во вышедших из системы заявок:\t%zu\n"
        "Среднее время пребывания в очереди:\t%.3lf е.в.\n"
        "===========================================================\n"
        ANSI_COLOR_RESET,
        modelT,
        numOfServ,
        modelT - waitT,
        (modelT - waitT) / modelT * 100,
        waitT,
        waitT / modelT * 100,
        openedReq,
        closedReq,
        sumTimeInQueue / (double)numOfServ);
}

void printStatShort(const size_t numOfServ, const size_t queueLen, const double avgQueueLen) {
    printf(
        "\n===============================\n"
        "Обработано заявок:\t%zu\n"
        "Текущая длина очереди:\t%zu\n"
        "Средняя длина очереди:\t%.3lf\n"
        "================================\n",
        numOfServ,
        queueLen,
        avgQueueLen);
}


static double g_errorIn = 0.0, g_errorOut = 0.0;
static size_t repeatsForCalcErr = 1000;

void calcError(double modelT, double waitT, size_t openReq, size_t numOfServ) {
    double avgDeliverT = (timeConstants.delayMax + timeConstants.delayMin) / 2;
    double avgHandlT = (timeConstants.handlTimeMax + timeConstants.handlTimeMin) / 2;

    double expOpenReq = modelT / avgDeliverT;
    double errorIn = relativeError(expOpenReq, openReq);

    double expWorkT = numOfServ * avgHandlT; 
    double errorOut = relativeError(expWorkT, modelT - waitT);

    g_errorIn += errorIn;
    g_errorOut += errorOut;
}

void errVariablesReset() {
    g_errorOut = 0.0;
    g_errorIn = 0.0;
}

size_t getRepeatsForCalcErr() {
    return repeatsForCalcErr;
}

void printCalcError() {
    printf(
        ANSI_COLOR_BLUE
        "======Оценка погрешности=======\n"
        ANSI_COLOR_YELLOW
        "По входу:\t%.5lf%%\n"
        "По выходу:\t%.5lf%%\n"
        "===============================\n"
        ANSI_COLOR_RESET,
        g_errorIn / repeatsForCalcErr,
        g_errorOut / repeatsForCalcErr);  
}


