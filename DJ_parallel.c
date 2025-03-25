#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <omp.h>

#define INF INT_MAX

void dijkstra_parallel(int **graph, int V, int src, int num_threads, int chunk_size, int *dist, bool *visited) {
    // Initialize distances and visited array
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;

    omp_set_num_threads(num_threads); // Set OpenMP threads

    for (int count = 0; count < V - 1; count++) {
        int min = INF, u = -1;

        // Parallel selection of the minimum distance node using reduction
        #pragma omp parallel for reduction(min: min) reduction(||: u) schedule(dynamic, chunk_size)
        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        // Parallel update of distances
        #pragma omp parallel for schedule(dynamic, chunk_size)
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF 
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
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

    int num_threads, chunk_size;

    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    printf("Enter the chunk size: ");
    scanf("%d", &chunk_size);

    // Allocate memory for dist and visited arrays outside the loop
    int *dist = (int *)malloc(V * sizeof(int));
    bool *visited = (bool *)malloc(V * sizeof(bool));

    int iterations = 10;
    double total_time = 0.0;

    for (int i = 0; i < iterations; i++) {
        double start_time = omp_get_wtime();

        dijkstra_parallel(graph, V, 0, num_threads, chunk_size, dist, visited);

        double end_time = omp_get_wtime();
        double time_taken = end_time - start_time;
        total_time += time_taken;

        printf("Iteration %d Execution Time: %.6f seconds\n", i + 1, time_taken);
    }

    // Print final shortest distances
    printf("\nVertex \t Distance from Source 0\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);

    printf("\nAverage Execution Time over %d runs: %.6f seconds\n", iterations, total_time / iterations);

    // Free allocated memory
    free(dist);
    free(visited);
    for (int i = 0; i < V; i++)
        free(graph[i]);
    free(graph);

    return 0;
}
