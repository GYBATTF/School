/**
 * @author Alexander Harms
 * Program to calculate prime factors of two numbers,
 * then find common factors
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * Displays an array to stdout
 * @param A
 * 0 terminated array to display
 */
void show_array(unsigned *A) {
    // Base and edge case
    if (A == 0 || *A == 0) return;
    // Print element
    printf("%d%s", *A, *(A + 1) == 0 ? "" : ", ");
    // Print next element
    show_array(A + 1);
}

/**
 * Builds an array containing common elements in both arrays
 * @param A
 * 0 terminated array
 * @param B
 * 0 terminated array
 * @return
 * array of common elements
 */
unsigned* build_common(unsigned *A, unsigned *B) {
    // Edge cases
    if (A == 0 || B == 0) return 0;
    // Bleed arrays until until elements are equal or not 0
    while (*A < *B && *A != 0) A++;
    while (*B < *A && *B != 0) B++;
    // Base case
    if (*A == 0 || *B == 0) return calloc(sizeof(unsigned), 1);

    unsigned *a = malloc(sizeof(unsigned)), s;

    // Add common elements
    for (s = 0; *A == *B; *(a + s) = 0, A++, B++) {
        *(a + s++) = *A;
        a = realloc(a, sizeof(unsigned) * (s + 1));
    }

    // Get next set of common elements
    unsigned *b = build_common(A, B);
    // Add other common elements to this array
    for (unsigned *c = b; *c != 0; *(a + s) = 0) {
        *(a + s++) = *c++;
        a = realloc(a, sizeof(unsigned) * (s + 1));
    }
    // Clean up
    free(b);

    return a;
}

/**
 * Builds a list of prime factors for the given number
 * @param N
 * the number to get factors for
 * @return
 * an array of factors
 */
unsigned* build_factors(unsigned N) {
    // Check for prime
    for (int i = 2, s = 0; i < N / 2; i++) if (N % i == 0) {
        // Get arrays of factors
        unsigned *a = build_factors(i);
        unsigned *b = build_factors(N / i);
        unsigned *c = malloc(sizeof(unsigned));

        // Merge factor array
        for (int j = 0, k = 0; *(a + j) != 0 || *(b + k) != 0; *(c + s) = 0) {
            // Add smaller element to array c
            if (*(a + j) <= *(b + k) && *(a + j) != 0) *(c + s++) = *(a + j++);
            else if (*(b + k) < *(a + j) || *(b + k) != 0) *(c + s++) = *(b + k++);
            // Grow array c
            c = realloc(c, sizeof(unsigned) * (s + 1));
        }

        // Clean up
        free(a); free(b);
        return c;
    }

    // Prime
    unsigned *a = calloc(2, sizeof(unsigned));
    *a = N;
    // Check edge case
    return  N < 1 ? 0 : a;
}

/**
 * Main function to drive user input and output
 * @return
 * 0 for success
 */
int main() {
    // Get numbers
    unsigned *mn = malloc(sizeof(unsigned) * 2);
    printf("Enter two integers M, N:\n");
    scanf("%d, %d", mn, mn + 1);

    // Make calculations
    unsigned **mnc = malloc(sizeof(unsigned*) * 3);
    *mnc = build_factors(*mn);
    *(mnc + 1) = build_factors(*(mn + 1));
    *(mnc + 2) = build_common(*mnc, *(mnc + 1));

    // Print results
    for (int i = 0; i < 3; i++, printf("\n")) {
        if (i == 2) printf("%d and %d have common factors: ", *mn, *(mn + 1));
        if (i <= 1) printf("%d has factors: ", *(mn + i));
        show_array(*(mnc + i));
    }

    // Cleanup
    for (int i = 0; i < 3; i++) free(*(mnc + i));
    free(mn); free(mnc);
    return 0;
}
