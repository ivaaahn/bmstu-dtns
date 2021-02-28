#ifndef _HANDLING_H_
#define _HANDLING_H_

#include <stdbool.h>

#include "queue.h"
#include "utils.h"

int modeling(Queue* queue, qkind_t qkind, bool test, size_t *maxLen);
int analyze(Queue *queue);
int changeFlagPrintAddr();
int changeFlagPrintErr();
void calcErrorWrapper(Queue *queue, qkind_t qkind);
bool isPrintAddr();
bool isPrintErr();


#endif  // _HANDLING_H_