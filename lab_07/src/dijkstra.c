#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#include "defines.h"
#include "graph.h"
#include "dijkstra.h"


static int minVertex(int *dist, bool visited[], int n) { 
    int minDist = INF, minVertex; 
  
    for (int v = 0; v < n; v++) 
        if (!visited[v] && dist[v] < minDist) 
            minDist = dist[v], minVertex = v; 
  
    return minVertex; 
} 


int *Dijkstra(Graph *graph, int start) {
    int n = graph->size;

    int *dist = malloc(sizeof(int) * n);
    bool visited[n];
  
    for (int i = 0; i < n; i++) 
        dist[i] = INF, visited[i] = false; 
  
    dist[start] = 0; 
  
    for (int count = 0; count < n - 1; count++) { 
        int u = minVertex(dist, visited, n);   
        visited[u] = true; 
  
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph->matrix[u][v] != INF && dist[u] != INF
                && dist[u] + graph->matrix[u][v] < dist[v]) 
                dist[v] = dist[u] + graph->matrix[u][v]; 
        }
    } 

    return dist;
}