#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000000 // Size of the array

int main() {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int)); // Allocate memory for the array
    int sum = 0; // Variable to store the sum

    // Initialize the array with random values
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100; // Random values between 0 and 99
    }

    // Start timing
    clock_t start = clock();

    // Calculate the sum sequentially
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }

    // Stop timing
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate time taken

    // Print the result and execution time
    printf("Sequential Sum: %d\n", sum);
    printf("Time taken: %f seconds\n", time_taken);

    // Free allocated memory
    free(array);

    return 0;
}