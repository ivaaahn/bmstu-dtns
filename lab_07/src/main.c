#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

#include "defines.h"
#include "graph.h"
#include "dijkstra.h"


int main(int argc, char *argv[]) {
    T_INIT;
    if (argc < 2) {
        ERR_LAUNCH_VERB;
    }

    FILE *istream = fopen(argv[1], "r");
    if (!istream) {
        ERR_FOPEN_VERB;
    }

    
    int graphSize;
    if (fscanf(istream, "%d", &graphSize) != 1 || graphSize < 1) {
        fclose(istream);
        ERR_INPUT_VERB;
    }

    printf("Введите стартовую вершину [%d - %d] и максимальную длину (А) (натуральные числа): ", 0, graphSize - 1);
    int startNode, maxDist;
    if (scanf("%d%d", &startNode, &maxDist) != 2 || startNode < 0 || startNode > graphSize - 1 || maxDist < 0) {
        fclose(istream);
        ERR_INPUT_VERB;
    }
    
    
    Graph *graph = createGraph(graphSize);
    if (readGraph(graph, istream) != OK) {
        fclose(istream);
        ERR_INPUT_VERB;
    }

    T_BEGIN;
    int *dist = Dijkstra(graph, startNode);
    T_END;
   
    system("clear");
    for (int i = 0; i < graphSize; i++) {
        if (dist[i] != INF) {
            printf("Расстояние (%d) -> (%d) == %d", startNode, i, dist[i]);
            if (dist[i] <= maxDist)
                printf(ANSI_COLOR_GREEN" <- ПОДХОДИТ\n"ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_RED" <- НЕ ПОДХОДИТ\n"ANSI_COLOR_RESET);
        }
        else {
            printf("Расстояние (%d) -> (%d) == INF", startNode, i);
            printf(ANSI_COLOR_RED" <- НЕ ПОДХОДИТ\n"ANSI_COLOR_RESET);
        }
    }
    printf("Время: %.0f нс\n", DELTA);
    printf("Граф: %zu байт\nМассив посещенных вершин: %zu байт\nМассив расстояний: %zu байт\n", sizeof(Graph *) \
        + sizeof(Graph) + sizeof(int) * graphSize * graphSize, sizeof(bool) * graphSize, sizeof(int) * graphSize);
    
    ExportWrapper(graph);
    delGraph(graph);
    fclose(istream);
    free(dist);
    system("rm data/graphView");
    return 0;
}
