#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h> // For high-precision time measurement

#define INF INT_MAX

void dijkstra(int **graph, int V, int src, int *dist, bool *visited) {
    // Initialize distances and visited array
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        // Find the vertex with the minimum distance
        int min = INF, u = -1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // No more reachable nodes
        visited[u] = true;

        // Update the distances to adjacent vertices
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF 
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

// Function to get time in milliseconds
double get_time_in_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0); // Convert to milliseconds
}

int main() {
    int V, num_threads, chunk_size;

    // User input for dynamic values
    printf("Enter number of vertices: ");
    scanf("%d", &V);

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    printf("Enter chunk size: ");
    scanf("%d", &chunk_size);

    // Dynamically allocate graph
    int **graph = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
        graph[i] = (int *)malloc(V * sizeof(int));

    // Generating a random adjacency matrix for testing (weighted graph)
    printf("\nGenerated Adjacency Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                graph[i][j] = 0; // No self-loops
            } else {
                graph[i][j] = (rand() % 100) + 1; // Random weight (1-100)
                graph[j][i] = graph[i][j]; // Symmetric for undirected graph
            }
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }

    // Allocate dist and visited arrays once
    int *dist = (int *)malloc(V * sizeof(int));
    bool *visited = (bool *)malloc(V * sizeof(bool));

    double start = get_time_in_milliseconds();
    dijkstra(graph, V, 0, dist, visited);
    double end = get_time_in_milliseconds();

    // Print results
    printf("\nVertex \t Distance from Source 0\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);

    printf("\nExecution Time: %.3f ms\n", end - start);

    // Free allocated memory
    free(dist);
    free(visited);
    for (int i = 0; i < V; i++)
        free(graph[i]);
    free(graph);

    return 0;
}

