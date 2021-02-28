#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "graph.h"
#include "defines.h"

Graph *createGraph(int n) {
    int **data = malloc(n * sizeof(int*) + n * n * sizeof(int));
    for (int i = 0; i < n; i++)
        data[i] = (int*)((char*)data + n * sizeof(int*) + i * n * sizeof(int));

    Graph *graph = malloc(sizeof(Graph));
    graph->matrix = data;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            data[i][j] = INF;

    graph->size = n;

    return graph;
}

void delGraph(Graph *graph) {
    free(graph->matrix);
    free(graph);
}


int readGraph(Graph *graph, FILE *istream) {  
    int i, j;

    for (i = 0; i < graph->size; i++)
        for (j = 0; (j < graph->size) && (fscanf(istream, "%d", &graph->matrix[i][j]) == 1); j++) {
            if (graph->matrix[i][j] == -1) {
                graph->matrix[i][j] = INF;
            }
        }
    
    
    if (i * j != graph->size * graph->size)
        return FAIL;
        
    return OK;
}

static void toDot(Graph *graph, FILE *f) {
    int n = graph->size;
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            if (graph->matrix[i][j] != INF && i != j) 
                fprintf(f, "%d -> %d [label=\"%d\"];\n", i, j, graph->matrix[i][j]); 
}

static void ExportToDot(FILE *stream, const char *treeName, Graph *graph) {
    fprintf(stream, "digraph %s {\n", treeName);
    toDot(graph, stream);
    fprintf(stream, "}\n");
}


int ExportWrapper(Graph *graph) {
    const char *ostream_name = "data/graphView";
    FILE *ostream = fopen(ostream_name, "w");
    ExportToDot(ostream, "GraphView", graph);
    fclose(ostream);

    char sysReq[128];
    snprintf(sysReq, sizeof(sysReq), 
    "dot -Tpng %s -o %s.png &&"
    "chmod +x ./%s.png &&"
    "code ./%s.png", 
    ostream_name, 
    ostream_name, 
    ostream_name, 
    ostream_name);
    
    system(sysReq);
    return OK;
}