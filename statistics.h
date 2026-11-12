#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "grade.h"

#define GRADES_MAX_RECORDS 100
#define GRADES_MAX_NAME_LENGTH 63
#define LINE_LENGTH 256


typedef enum {
    LETTER_A,
    LETTER_B,
    LETTER_C,
    LETTER_D,
    LETTER_F,
    LETTER_GRADE_COUNT
} LetterGrade;

typedef struct {
    size_t count;
    double mean;
    double median;
    double minimum;
    double maximum;
    double standard_deviation;
    size_t minimum_index;
    size_t maximum_index;
    size_t letter_counts[LETTER_GRADE_COUNT];
} GradeStatistics;



bool statistics_calculate(const GradeBook *grade_book,
                                 GradeStatistics *stats);

#endif
