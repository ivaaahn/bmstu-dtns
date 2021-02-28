#include <stdlib.h>
#include <stdbool.h>

#include "arr_queue.h"
#include "list_queue.h"
#include "defines.h"
#include "queue.h"



Queue *queueCreateAll() {
    Queue *queue = malloc(sizeof(Queue));
    queueCreate(queue, Q_ARRAY);
    queueCreate(queue, Q_LIST);
    return queue;
}

void queueDeleteAll(Queue *queue) {
    queueDelete(queue, Q_LIST);
    queueDelete(queue, Q_ARRAY);
    free(queue);
}

void queueCreate(Queue *queue, const qkind_t qkind) {
    if (qkind == Q_ARRAY) {
        queue->arrQueue = arrQueueCreate(timeConstants.targetAmount * 2);
    } else {
        queue->listQueue = listQueueCreate();
    }
}


void queueDelete(Queue *queue, const qkind_t qkind) {
    if (qkind == Q_ARRAY) {
        arrQueueDelete(queue->arrQueue);
    } else {
        listQueueDelete(queue->listQueue);
    }
}


void queueReset(Queue *queue, const qkind_t qkind) {
    if (qkind == Q_ARRAY) {
        arrQueueReset(queue->arrQueue);
    } else {
        listQueueReset(queue->listQueue);
    }
}

int enQueue(Queue *queue, const qkind_t qkind, const qtype_t *item) {
    bool rc = (qkind == Q_ARRAY) ? \
    enArrQueue(queue->arrQueue, item) : \
    enListQueue(queue->listQueue, item);

    return (rc == true) ? OK : FAIL;
}

int deQueue(Queue *queue, const qkind_t qkind, qtype_t *item) {
    bool rc = (qkind == Q_ARRAY) ? \
    deArrQueue(queue->arrQueue, item) : \
    deListQueue(queue->listQueue, item);

    return (rc == true) ? OK : FAIL;
}

bool isEmptyQueue(const Queue *queue, const qkind_t qkind) {
    return qkind == Q_ARRAY ? \
    arrQueueIsEmpty(queue->arrQueue) : \
    listQueueIsEmpty(queue->listQueue);
}


void getQueueItem(const Queue *queue, qtype_t *item, const qkind_t qkind) {
    if (qkind == Q_ARRAY) {
        getArrQueueItem(queue->arrQueue, item);
    } else {
        getListQueueItem(queue->listQueue, item);
    }
}

size_t getQueueLength(const Queue *queue, const qkind_t qkind) {
    return qkind == Q_ARRAY ? \
    queue->arrQueue->length : \
    queue->listQueue->length;
} 