#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000000  // Number of Fibonacci terms
#define RUNS 10  // Number of times to execute

void fibonacciSequential(long long *fib) {
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < N; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int main() {
    // Allocate memory on the heap
    long long *fib = (long long *)malloc(N * sizeof(long long));
    if (fib == NULL) {
        printf("Memory allocation failed!\n");
        return 1;  // Exit with an error
    }

    double total_time = 0.0;

    for (int i = 0; i < RUNS; i++) {
        clock_t start, end;
        start = clock();

        fibonacciSequential(fib);

        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;

        printf("Run %d: Execution Time = %f seconds\n", i + 1, time_taken);
    }

    double avg_time = total_time / RUNS;
    printf("\nAverage Execution Time over %d runs: %f seconds\n", RUNS, avg_time);

    free(fib); // Free allocated memory
    return 0;
}
