/**
 * Prints the counts of characters going in/out
 * @author ajharms
 */
#include <stdio.h>

#include "print_counts.h"
#include "../supplied/encrypt.h"

/**
 * Print the counts of characters being read and written
 */
void printCounts() {
    printf("Input file contains\n");
    for (char i = 'A'; i <= 'Z'; i++)
        printf("%c:%d%s", i, get_input_count(i), i == 'Z' ? "" : " ");

    printf("\nOutput file contains\n");
    for (char i = 'A'; i <= 'Z'; i++)
        printf("%c:%d%s", i, get_output_count(i), i == 'Z' ? "" : " ");

    printf("\n");
}