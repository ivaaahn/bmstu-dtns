#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include "request.h"
#include "arr_queue.h"
#include "list_queue.h"


typedef Request qtype_t;
typedef enum {
    Q_LIST,
    Q_ARRAY
    // Q_COUNT,
    // Q_ERR
} qkind_t;

typedef struct {
    ArrQueue *arrQueue;
    ListQueue *listQueue;
} Queue;

Queue *queueCreateAll();
void queueDeleteAll(Queue *queue);

void queueCreate(Queue *queue, const qkind_t qkind);
void queueDelete(Queue *queue, const qkind_t qkind);

int enQueue(Queue *queue, const qkind_t qkind, const qtype_t *item);
int deQueue(Queue *queue, const qkind_t qkind, qtype_t *item);

bool isEmptyQueue(const Queue *queue, const qkind_t qkind);

void getQueueItem(const Queue *queue, qtype_t *item, const qkind_t qkind);
size_t getQueueLength(const Queue *queue, const qkind_t qkind);

void queueReset(Queue *queue, const qkind_t qkind);


#endif  // _QUEUE_H_