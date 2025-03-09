#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h> // For high-precision time measurement

#define INF INT_MAX

void dijkstra(int **graph, int V, int src) {
    int *dist = (int *)malloc(V * sizeof(int));
    bool *visited = (bool *)malloc(V * sizeof(bool));

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int min = INF, u = -1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF 
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("Vertex \t Distance from Source %d\n", src);
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);

    free(dist);
    free(visited);
}

double get_time_in_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0); // Convert to milliseconds
}

int main() {
    int V = 5;
    int **graph = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
        graph[i] = (int *)malloc(V * sizeof(int));

    int adjacencyMatrix[5][5] = {
        {0, 10, 0, 30, 100},
        {10, 0, 50, 0, 0},
        {0, 50, 0, 20, 10},
        {30, 0, 20, 0, 60},
        {100, 0, 10, 60, 0}
    };

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            graph[i][j] = adjacencyMatrix[i][j];

    int iterations = 10;
    double total_time = 0.0;

    for (int i = 0; i < iterations; i++) {
        double start = get_time_in_milliseconds();

        dijkstra(graph, V, 0);

        double end = get_time_in_milliseconds();
        double time_taken = end - start;
        total_time += time_taken;

        printf("Iteration %d Execution Time: %.3f ms\n", i + 1, time_taken);
    }

    double avg_time = total_time / iterations;
    printf("\nAverage Execution Time over %d runs: %.3f ms\n", iterations, avg_time);

    for (int i = 0; i < V; i++)
        free(graph[i]);
    free(graph);

    return 0;
}
