#ifndef COURSE
#define COURSE
#include "sDB.h"

typedef struct CourseNode {
    int courseCode;
    int marks;
    struct CourseNode* prev;
    struct CourseNode* next;
} CourseNode;



CourseNode* createCourse(int courseCode, int marks);
void addCourse(StudentDatabase* db, int rollNo, int courseCode, int marks);
void addCourseInitial(StudentDatabase* db, int rollNo, int courseCode, int marks);
void modifyCourse(StudentDatabase* db, int rollNo, int courseCode, int newMarks);
void deleteCourse(StudentDatabase* db, int rollNo, int courseCode);

#endif
