
 //=================================================
 // Includes and Defines
 //=================================================
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grade.h"
#include "statistics.h"



 //=================================================
 // Compare Doubles Function
 //=================================================
static int compare_doubles(const void *left, const void *right){
    const double a = *(const double *)left;
    const double b = *(const double *)right;
    return (a > b) - (a < b);
}


 //=================================================
 // Compute Letter Grade Function
 //=================================================
static LetterGrade letter_grade(double score){
    if (score >= 90.0) return LETTER_A;
    if (score >= 80.0) return LETTER_B;
    if (score >= 70.0) return LETTER_C;
    if (score >= 60.0) return LETTER_D;
    return LETTER_F;
}


 //=================================================
 // Compute Statistics Function
 //=================================================
bool statistics_calculate(const GradeBook *grade_book,
                                 GradeStatistics *stats){
    double sorted_scores[GRADES_MAX_RECORDS];
    double sum = 0.0;
    double squared_difference_sum = 0.0;
    size_t i;

    if (grade_book == NULL || stats == NULL || grade_book->count == 0) {
        return false;
    }

    stats->count = grade_book->count;
    stats->minimum = grade_book->records[0].score;
    stats->maximum = grade_book->records[0].score;
    stats->minimum_index = 0;
    stats->maximum_index = 0;
    for (i = 0; i < LETTER_GRADE_COUNT; ++i) {
        stats->letter_counts[i] = 0;
    }

    for (i = 0; i < grade_book->count; ++i) {
        const double score = grade_book->records[i].score;
        sorted_scores[i] = score;
        sum += score;
        ++stats->letter_counts[letter_grade(score)];
        if (score < stats->minimum) {
            stats->minimum = score;
            stats->minimum_index = i;
        }
        if (score > stats->maximum) {
            stats->maximum = score;
            stats->maximum_index = i;
        }
    }

    stats->mean = sum / (double)grade_book->count;
    qsort(sorted_scores, grade_book->count, sizeof sorted_scores[0],
          compare_doubles);
    if (grade_book->count % 2 == 1) {
        stats->median = sorted_scores[grade_book->count / 2];
    } else {
        const size_t upper = grade_book->count / 2;
        stats->median = (sorted_scores[upper - 1] + sorted_scores[upper]) / 2.0;
    }

    for (i = 0; i < grade_book->count; ++i) {
        const double difference = grade_book->records[i].score - stats->mean;
        squared_difference_sum += difference * difference;
    }
    stats->standard_deviation =
        sqrt(squared_difference_sum / (double)grade_book->count);
    return true;
}


