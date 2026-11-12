#ifndef _GRADE_H_
#define _GRADE_H_

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



bool grades_load(const char *filename, GradeBook *grade_book);

#endif
