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
bool grades_load(const char *filename, GradeBook *grade_book){
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


