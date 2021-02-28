#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>

typedef struct {
    int **matrix;
    int size;
} Graph;



Graph *createGraph(int n);
int readGraph(Graph *graph, FILE *istream);
void delGraph(Graph *graph);

int ExportWrapper(Graph *graph);

#endif  // _GRAPH_H_