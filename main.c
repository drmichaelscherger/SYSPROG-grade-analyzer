// Mike Scherger
/*
 * Grade Analyzer
 *
 * This program intentionally begins as one source file. During the lab, you
 * will preserve its behavior while separating it into cohesive modules.
 */


 //=================================================
 // Includes and Defines
 //=================================================
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "grade.h"
#include "statistics.h"



 //=================================================
 // Print Usage Function
 //=================================================
static void print_usage(const char *program_name){
    fprintf(stderr, "Usage: %s <grades.csv>\n", program_name);
}


 //=================================================
 // Main Function
 //=================================================
int main(int argc, char *argv[]){
    GradeBook grade_book;
    GradeStatistics stats;

    if (argc != 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (!grades_load(argv[1], &grade_book)) {
        return EXIT_FAILURE;
    }

    if (!statistics_calculate(&grade_book, &stats)) {
        fprintf(stderr, "error: no grades to analyze\n");
        return EXIT_FAILURE;
    }

    printf("Grade report for %s\n", argv[1]);
    printf("----------------------------\n");
    printf("Students: %zu\n", stats.count);
    printf("Mean:     %.2f\n", stats.mean);
    printf("Median:   %.2f\n", stats.median);
    printf("Minimum:  %.2f (%s)\n", stats.minimum,
           grade_book.records[stats.minimum_index].name);
    printf("Maximum:  %.2f (%s)\n", stats.maximum,
           grade_book.records[stats.maximum_index].name);
    printf("Std dev:  %.2f\n", stats.standard_deviation);
    printf("\nLetter-grade distribution\n");
    printf("A: %zu\n", stats.letter_counts[LETTER_A]);
    printf("B: %zu\n", stats.letter_counts[LETTER_B]);
    printf("C: %zu\n", stats.letter_counts[LETTER_C]);
    printf("D: %zu\n", stats.letter_counts[LETTER_D]);
    printf("F: %zu\n", stats.letter_counts[LETTER_F]);

    return EXIT_SUCCESS;
}
