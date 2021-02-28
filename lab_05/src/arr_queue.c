#include <stdlib.h>
#include <stdbool.h>

#include "arr_queue.h"


ArrQueue *arrQueueCreate(const size_t capacity) {
    ArrQueue *queue = malloc(sizeof(ArrQueue));
    
    queue->capacity = capacity;
    queue->buf = calloc(capacity, sizeof(aqtype_t));
    queue->head = queue->tail = 0;
    queue->length = 0;

    return queue;
}

void arrQueueReset(ArrQueue *queue) {
    queue->head = queue->tail = queue->length = 0;
}


ArrQueue *arrQueueResize(ArrQueue *queue, const size_t newCapacity) {

    queue->buf = realloc(queue->buf, sizeof(newCapacity * sizeof(aqtype_t)));
    queue->capacity = newCapacity;
    queue->head = queue->tail = 0;
    queue->length = 0;

    return queue;
}

void arrQueueDelete(ArrQueue *queue) {
    free(queue->buf);
    free(queue);
}

bool arrQueueIsFull(const ArrQueue *queue) {
    return queue->length == queue->capacity;
}

bool arrQueueIsEmpty(const ArrQueue *queue) {
    return !(queue->length);
}

bool enArrQueue(ArrQueue *queue, const aqtype_t *item) {
    if (arrQueueIsFull(queue)) {
        return false;
    }

    queue->buf[queue->tail] = *item;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->length++;

    return true;
}

bool deArrQueue(ArrQueue *queue, aqtype_t *item) {
    if (arrQueueIsEmpty(queue)) {
        return false;
    }

    if (item) { 
        *item = queue->buf[queue->head];
    }

    queue->head = (queue->head + 1) % queue->capacity;
    queue->length--;

    return true;
}


void getArrQueueItem(const ArrQueue *queue, aqtype_t *item) {
    *item = queue->buf[queue->head];
}

size_t getArrQueueLength(const ArrQueue *queue) {
    return queue->length;
}

size_t getArrQueueMemSize(const size_t capacity) {
    return sizeof(ArrQueue*) + sizeof(ArrQueue) + sizeof(aqtype_t) * capacity;
}


