#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <omp.h>

#define INF INT_MAX

void dijkstra_parallel(int **graph, int V, int src, int num_threads) {
    int *dist = (int *)malloc(V * sizeof(int));
    bool *visited = (bool *)malloc(V * sizeof(bool));

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;

    omp_set_num_threads(num_threads); // Set the number of threads

    for (int count = 0; count < V - 1; count++) {
        int min = INF, u = -1;

        // Parallel minimum distance selection with static scheduling
        #pragma omp parallel
        {
            int local_min = INF, local_u = -1;
            int thread_id = omp_get_thread_num();

            #pragma omp for schedule(static) nowait
            for (int v = 0; v < V; v++) {
                if (!visited[v] && dist[v] < local_min) {
                    local_min = dist[v];
                    local_u = v;
                }
            }

            #pragma omp critical
            {
                if (local_min < min) {
                    min = local_min;
                    u = local_u;
                    printf("Thread %d selected node %d with min distance %d\n", thread_id, u, min);
                }
            }
        }

        if (u == -1) break;
        visited[u] = true;

        // Parallel distance update with static scheduling
        #pragma omp parallel for schedule(static)
        for (int v = 0; v < V; v++) {
            int thread_id = omp_get_thread_num();
            if (!visited[v] && graph[u][v] && dist[u] != INF 
                && dist[u] + graph[u][v] < dist[v]) {
                printf("Thread %d updating distance of node %d\n", thread_id, v);
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("\nVertex \t Distance from Source %d\n", src);
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);

    free(dist);
    free(visited);
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

    int num_threads;

    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    double start_time = omp_get_wtime();
    dijkstra_parallel(graph, V, 0, num_threads);
    double end_time = omp_get_wtime();

    printf("\nExecution Time: %.3f seconds with %d threads\n", 
           (end_time - start_time), num_threads);

    for (int i = 0; i < V; i++)
        free(graph[i]);
    free(graph);

    return 0;
}
