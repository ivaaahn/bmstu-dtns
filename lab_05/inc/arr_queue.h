#ifndef _ARR_QUEUE_H_
#define _ARR_QUEUE_H_

#include <stdlib.h>
#include <stdbool.h>
#include "request.h"

typedef Request aqtype_t;

#pragma pack(push, 1)
typedef struct {
    aqtype_t *buf;
    size_t capacity;
    size_t length;
    size_t head, tail;
}ArrQueue;
#pragma pack(pop)


ArrQueue *arrQueueCreate(const size_t capacity);
void arrQueueReset(ArrQueue *queue);
void arrQueueDelete(ArrQueue *queue);

bool arrQueueIsFull(const ArrQueue *queue);
bool arrQueueIsEmpty(const ArrQueue *queue);

bool enArrQueue(ArrQueue *queue, const aqtype_t *item);
bool deArrQueue(ArrQueue *queue, aqtype_t *item);

void getArrQueueItem(const ArrQueue *queue, aqtype_t *item);
size_t getArrQueueLength(const ArrQueue *queue);
size_t getArrQueueMemSize(const size_t capacity);


#endif // _ARR_QUEUE_H_
