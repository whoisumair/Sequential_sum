#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000  // Problem size

int main() {
    int i;
    double sum = 0.0;
    double a[N], b[N], result[N];

    // Initialize arrays
    for (i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = (N - i) * 1.0;
    }

    // Parallel region with variable scope definitions
    #pragma omp parallel shared(a, b, result, sum) private(i)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // **Parallel Loop with Static Scheduling**
        #pragma omp for schedule(static, 10)
        for (i = 0; i < N; i++) {
            result[i] = a[i] + b[i];
        }

        // **Parallel Reduction for Sum**
        #pragma omp for reduction(+:sum)
        for (i = 0; i < N; i++) {
            sum += result[i];
        }

        // **Parallel Critical Section (Alternative Approach)**
        #pragma omp for
        for (i = 0; i < N; i++) {
            #pragma omp critical
            {
                sum += result[i];  // Avoids race conditions but less efficient than reduction
            }
        }

        // **Parallel Atomic (Alternative Approach)**
        #pragma omp for
        for (i = 0; i < N; i++) {
            #pragma omp atomic
            sum += result[i];  // Atomic operation to prevent data race
        }

        // **Dynamic Scheduling Example**
        #pragma omp for schedule(dynamic, 20)
        for (i = 0; i < N; i++) {
            result[i] *= 2.0;
        }
    }  // End parallel region

    printf("Final sum: %f\n", sum);
    return 0;
}
