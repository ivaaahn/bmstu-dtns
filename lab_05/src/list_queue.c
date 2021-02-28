#include <stdlib.h>
#include <stdbool.h>

#include "list_queue.h"


static Node *nodeCreate(const lqtype_t *item) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = *item;
    new_node->next = NULL;

    return new_node;
}

static void nodeDelete(Node *node) {
    free(node);
}

static void freeAllNodes(Node *head) {
    while (head) {
        Node *next = head->next;
        nodeDelete(head);
        head = next;
    }
}

ListQueue *listQueueCreate() {
    ListQueue *queue = malloc(sizeof(ListQueue));
    queue->head = queue->tail = NULL;
    queue->length = 0;

    return queue;
}

void listQueueReset(ListQueue *queue) {
    freeAllNodes(queue->head);
    queue->head = queue->tail = NULL;
    queue->length = 0;

}
void listQueueDelete(ListQueue *queue) {
    freeAllNodes(queue->head);
    free(queue);
}


bool listQueueIsEmpty(const ListQueue *queue) {
    return (!(queue->head));
}

bool enListQueue(ListQueue *queue, const lqtype_t *item) {
    if (queue->tail) {
        queue->tail->next = nodeCreate(item);
        queue->tail = queue->tail->next;
    } else {
        queue->tail = nodeCreate(item);
        queue->head = queue->tail;
    }

    queue->length++;

    return true;
}

bool deListQueue(ListQueue *queue, lqtype_t *item) {
    if (listQueueIsEmpty(queue)) {
        return false;
    }

    Node *address = queue->head;
   
    if (item) {
        *item = queue->head->data;
    }
   
    if (!(queue->head = queue->head->next)) {
        queue->tail = NULL;
    }
    
    nodeDelete(address);
    queue->length--;

    return true;
}


void getListQueueItem(const ListQueue *queue, lqtype_t *item) {
    *item = queue->head->data;
}

size_t getListQueueLength(const ListQueue *queue) {
    return queue->length;
}

size_t getListQueueMemSize(const size_t length) {
    return sizeof(ListQueue*) + sizeof(ListQueue) + length * sizeof(Node);
}