#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "arr_queue.h"
#include "list_queue.h"
#include "request.h"
#include "print_statistics.h"
#include "handling.h"

#include "queue.h"
#include "defines.h"
#include "utils.h"

static bool g_printAddr = false;
static bool g_printErr = false;
static FILE *addrFile = NULL;

bool isPrintAddr() { 
    return g_printAddr;
}

bool isPrintErr() {
    return g_printErr;
}

static void printDelAddr(const void *addr) { 
    fprintf(addrFile, "(%p) ->\n", addr); 
}

static void printNewAddr(const void *addr) {
    fprintf(addrFile, "-> (%p)\n", addr); 
}


int changeFlagPrintAddr() {
    if ((g_printAddr = !g_printAddr)) {
        addrFile = fopen("adresses_statistic", "w");
        fprintf(addrFile, "-> (Адрес) - добавленный элемент, (Адрес) -> - высвобожденный элемент\n");  
    } else if (addrFile) {
        fclose(addrFile);
        addrFile = NULL;
    }

    return OK;
}


int changeFlagPrintErr() {
    g_printErr = !g_printErr;

    return OK;
}

static bool again() { 
    return randomInt(1, 100000) <= timeConstants.probability * 100000; 
}

void calcErrorWrapper(Queue *queue, qkind_t qkind) {
    errVariablesReset();
    size_t repeats = getRepeatsForCalcErr();
    for (size_t i = 0; i < repeats; i++) {
        modeling(queue, qkind, true, NULL);
    }
    printCalcError();
}

int modeling(Queue* queue, qkind_t qkind, bool test, size_t *maxLen) {
    double currT = 0.0, waitT = 0.0, offset = 0.0, sumTimeInQueue = 0.0;
    size_t closedReq = 0, openedReq = 0, numOfServ = 0, lastPrintPoint = 0;
    size_t sumQueueLen = 0;
    qtype_t tempReq, currReq;

    requestGenerate(&tempReq, currT);
    while (closedReq < timeConstants.targetAmount) {
        if (isEmptyQueue(queue, qkind)) {
            waitT += tempReq.deliverT - currT;
            currT = tempReq.deliverT;
            openedReq++;    
            
            if (enQueue(queue, qkind, &tempReq) != OK) {
                queueReset(queue, qkind);
                return OVERFLOW; 
            }
            if (qkind == Q_LIST && isPrintAddr() && !test) { printNewAddr((void*)(queue->listQueue->tail)); }

            requestGenerate(&tempReq, currT);
        }

        getQueueItem(queue, &currReq, qkind);
        sumTimeInQueue += currT - currReq.deliverT;
        
        //  Смотрим, что произойдет, пока заявка будет обрабатываться
        while (tempReq.deliverT < currT + currReq.handlT) {
            openedReq++;

            if (enQueue(queue, qkind, &tempReq) != OK) {
                queueReset(queue, qkind);
                return OVERFLOW; 
            }
            if (qkind == Q_LIST && isPrintAddr() && !test) { printNewAddr((void*)(queue->listQueue->tail)); } 

            offset = tempReq.deliverT - currT;
            requestGenerate(&tempReq, currT + offset);
        } 

        offset = 0.0;

        if (qkind == Q_LIST && isPrintAddr() && !test) { printDelAddr((void*)(queue->listQueue->head)); }
        deQueue(queue, qkind, NULL);
        size_t currLen = getQueueLength(queue, qkind);
        numOfServ++;
        sumQueueLen += currLen;
        currT += currReq.handlT;  // завяка обработана 

        if (again()) {
            requestReGenerate(&currReq, currT);
            enQueue(queue, qkind, &currReq);
            if (qkind == Q_LIST && isPrintAddr() && !test) { printNewAddr((void*)(queue->listQueue->tail)); } 
        } else {
            closedReq++;
        }

        if (!test && (numOfServ != lastPrintPoint) && !(numOfServ % 100)) {
            lastPrintPoint = numOfServ;
            printStatShort(numOfServ, currLen, (double)sumQueueLen / numOfServ);
        }

        if (test && maxLen && *maxLen < currLen) { *maxLen = currLen; }
    }

    if (!test) {
        printStatAll(currT, waitT, numOfServ, closedReq, sumTimeInQueue, openedReq);
    } else if (test && !maxLen) {
        calcError(currT, waitT, openedReq, numOfServ);
    }

    queueReset(queue, qkind);
    return OK;
}


int analyze(Queue* queue) {
    int rc = OK;
    clock_t start;
    size_t maxLen = 0;

    const size_t repeats = 1000;


    printf(ANSI_COLOR_BLUE"#### Сравнительный анализ очереди, построенной на массиве и односвязном списке####\n"ANSI_COLOR_YELLOW);
    printf("Для каждого измерения взято среднее значение\n");
    printf("по времени для %4zu повторений\n\n"ANSI_COLOR_RESET, repeats);

    printf("|     Время (µs)      |       Объём памяти (байт)        |\n");
    printf("|  массив  |  список  |массив(исп)|массив(вся)|  список  |\n");
    printf("|----------|----------|-----------|-----------|----------|\n");


    clock_t full_arr = 0, full_list = 0;
    

     for (size_t rep = 0; rep < repeats; rep++) {
        start = clock();
        modeling(queue, Q_LIST, true, &maxLen);
        full_list += clock() - start;
    }

    for (size_t rep = 0; rep < repeats; rep++) {
        start = clock();
        if ((rc = modeling(queue, Q_ARRAY, true, &maxLen)) != OK) { break; }
        full_arr += clock() - start;
    }

    printf("|%10.3lf|%10.3lf|%11zu|%11zu|%10zu|\n"
           "----------------------------------------------------------\n",
            (double)full_arr / repeats, 
            (double)full_list / repeats,
            getArrQueueMemSize(maxLen),
            getArrQueueMemSize(timeConstants.targetAmount * 2),
            getListQueueMemSize(maxLen));


    return rc;
}
