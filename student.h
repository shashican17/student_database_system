#ifndef STUDENT
#define STUDENT

#include "sDB.h"
#include "course.h"

typedef struct StudentNode {
    int rollNo;
    char name[100];
    float cgpa;
    int numSubjects;
    CourseNode* coursesHead;
    struct StudentNode* prev;
    struct StudentNode* next;
} StudentNode;

StudentNode* createStudent(int rollNo, char* name, float cgpa, int numSubjects);
void addStudent(StudentDatabase* db, int rollNo, char* name, float cgpa, int numSubjects);
void modifyStudent(StudentDatabase* db, int rollNo, float newCgpa);
void deleteStudent(StudentDatabase* db, int rollNo);

#endif
