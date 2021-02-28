#ifndef _LIST_QUEUE_H_
#define _LIST_QUEUE_H_

#include <stdlib.h>
#include "request.h"

typedef Request lqtype_t; 

typedef struct Node Node;
struct Node {
    lqtype_t data;
    Node *next;
};

typedef struct {
    Node *head, *tail;
    size_t length;
} ListQueue;


ListQueue *listQueueCreate();
void listQueueDelete(ListQueue *queue);
void listQueueReset(ListQueue *queue);

bool listQueueIsEmpty(const ListQueue *queue);

bool enListQueue(ListQueue *queue, const lqtype_t *item);
bool deListQueue(ListQueue *queue, lqtype_t *item);

void getListQueueItem(const ListQueue *queue, lqtype_t *item);
size_t getListQueueMemSize(const size_t length);


#endif // _LIST_QUEUE_H_
