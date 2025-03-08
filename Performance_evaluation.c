#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000  // Problem size
#define NUM_RUNS 10  // Number of runs for averaging

void sequential_version(double *a, double *b, double *result, double *sum) {
    double start_time = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        result[i] = a[i] + b[i];
    }
    for (int i = 0; i < N; i++) {
        *sum += result[i];
    }

    double end_time = omp_get_wtime();
    printf("Sequential Time: %f seconds\n", end_time - start_time);
}

void parallel_version(double *a, double *b, double *result, double *sum) {
    double start_time = omp_get_wtime();

    #pragma omp parallel shared(a, b, result)
    {
        int i;

        // **Parallel Addition**
        #pragma omp for schedule(static, 10)
        for (i = 0; i < N; i++) {
            result[i] = a[i] + b[i];
        }

        // **Reduction for sum**
        #pragma omp for reduction(+:sum[0])
        for (i = 0; i < N; i++) {
            sum[0] += result[i];
        }
    }

    double end_time = omp_get_wtime();
    printf("Parallel Time: %f seconds\n", end_time - start_time);
}

int main() {
    double *a = (double *)malloc(N * sizeof(double));
    double *b = (double *)malloc(N * sizeof(double));
    double *result = (double *)malloc(N * sizeof(double));
    double sequential_time = 0.0, parallel_time = 0.0;

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = (N - i) * 1.0;
    }

    // Execute the sequential version multiple times
    for (int run = 0; run < NUM_RUNS; run++) {
        double sum = 0.0;
        double start = omp_get_wtime();
        sequential_version(a, b, result, &sum);
        double end = omp_get_wtime();
        sequential_time += (end - start);
    }
    sequential_time /= NUM_RUNS;  // Compute average time

    // Execute the parallel version multiple times
    for (int run = 0; run < NUM_RUNS; run++) {
        double sum = 0.0;
        double start = omp_get_wtime();
        parallel_version(a, b, result, &sum);
        double end = omp_get_wtime();
        parallel_time += (end - start);
    }
    parallel_time /= NUM_RUNS;  // Compute average time

    // **Performance Comparison**
    printf("\n=== Performance Comparison ===\n");
    printf("Average Sequential Time: %f seconds\n", sequential_time);
    printf("Average Parallel Time: %f seconds\n", parallel_time);
    printf("Speedup: %.2fx\n", sequential_time / parallel_time);

    // Cleanup
    free(a);
    free(b);
    free(result);

    return 0;
}