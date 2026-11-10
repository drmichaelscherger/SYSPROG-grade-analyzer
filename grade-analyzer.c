/*
 * Grade Analyzer
 *
 * This program intentionally begins as one source file. During the lab, you
 * will preserve its behavior while separating it into cohesive modules.
 */


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

#define GRADES_MAX_RECORDS 100
#define GRADES_MAX_NAME_LENGTH 63
#define LINE_LENGTH 256


 //=================================================
 // Type Definitions
 //=================================================
typedef struct {
    char name[GRADES_MAX_NAME_LENGTH + 1];
    double score;
} GradeRecord;

typedef struct {
    GradeRecord records[GRADES_MAX_RECORDS];
    size_t count;
} GradeBook;

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


 //=================================================
 // Trim Function
 //=================================================
static char *trim(char *text){
    char *end;

    while (isspace((unsigned char)*text)) {
        ++text;
    }

    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        --end;
    }
    *end = '\0';
    return text;
}


 //=================================================
 // Parse Record Function
 //=================================================
static bool parse_record(char *line, GradeRecord *record){
    char *comma = strchr(line, ',');
    char *name;
    char *score_text;
    char *end;
    double score;

    if (comma == NULL || strchr(comma + 1, ',') != NULL) {
        return false;
    }

    *comma = '\0';
    name = trim(line);
    score_text = trim(comma + 1);
    if (*name == '\0' || strlen(name) > GRADES_MAX_NAME_LENGTH) {
        return false;
    }

    errno = 0;
    score = strtod(score_text, &end);
    if (errno != 0 || end == score_text || *trim(end) != '\0' ||
        score < 0.0 || score > 100.0) {
        return false;
    }

    strcpy(record->name, name);
    record->score = score;
    return true;
}


 //=================================================
 // Load Grades from File Function
 //=================================================
static bool grades_load(const char *filename, GradeBook *grade_book){
    FILE *input;
    char line[LINE_LENGTH];
    size_t line_number = 0;

    if (filename == NULL || grade_book == NULL) {
        return false;
    }

    input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "error: cannot open '%s': %s\n", filename,
                strerror(errno));
        return false;
    }

    grade_book->count = 0;
    while (fgets(line, sizeof line, input) != NULL) {
        char *text;
        ++line_number;

        if (strchr(line, '\n') == NULL && !feof(input)) {
            fprintf(stderr, "error: %s:%zu: line is too long\n", filename,
                    line_number);
            fclose(input);
            return false;
        }

        text = trim(line);
        if (*text == '\0' || *text == '#') {
            continue;
        }
        if (grade_book->count == GRADES_MAX_RECORDS) {
            fprintf(stderr, "error: %s: more than %d records\n", filename,
                    GRADES_MAX_RECORDS);
            fclose(input);
            return false;
        }
        if (!parse_record(text, &grade_book->records[grade_book->count])) {
            fprintf(stderr,
                    "error: %s:%zu: expected name,score (score 0-100)\n",
                    filename, line_number);
            fclose(input);
            return false;
        }
        ++grade_book->count;
    }

    if (ferror(input)) {
        fprintf(stderr, "error: could not read '%s'\n", filename);
        fclose(input);
        return false;
    }

    fclose(input);
    return true;
}


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
static bool statistics_calculate(const GradeBook *grade_book,
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
