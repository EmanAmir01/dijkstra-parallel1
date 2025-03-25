#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000  // Number of Fibonacci terms
#define RUNS 1  // Number of times to execute

void fibonacciParallel(long long *fib) {
    fib[0] = 0;
    fib[1] = 1;

    #pragma omp parallel for schedule(dynamic,6)
    for (int i = 2; i < N; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int main() {
    // Allocate memory on the heap
    long long *fib = (long long *)malloc(N * sizeof(long long));
    if (fib == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    double total_time = 0.0;
    omp_set_num_threads(12); // Adjust based on CPU

    for (int i = 0; i < RUNS; i++) {
        double start = omp_get_wtime();

        fibonacciParallel(fib);

        double end = omp_get_wtime();
        double time_taken = end - start;
        total_time += time_taken;

        printf("Run %d: Execution Time = %f seconds\n", i + 1, time_taken);
    }

    double avg_time = total_time / RUNS;
    printf("\nAverage Execution Time over %d runs: %f seconds\n", RUNS, avg_time);

    free(fib); // Free memory
    return 0;
}
