#include "course.h"
#include "database.h"
#include "student.h"
#include <stdlib.h>
#include <stdio.h>

CourseNode* createCourse(int courseCode, int marks) {
    CourseNode* course = (CourseNode*)malloc(sizeof(CourseNode));
    course->courseCode = courseCode;
    course->marks = marks;
    course->prev = NULL;
    course->next = NULL;
    return course;
}

void addCourseInitial(StudentDatabase* db, int rollNo, int courseCode, int marks) {
    if (db == NULL || db->head == NULL) {
        printf("Error: Database or student list is empty\n");
        return;
    }

    StudentNode* student = db->head;
    while (student && student->rollNo != rollNo) {
        student = student->next;
    }

    if (student == NULL) {
        printf("Error: Student with roll number %d not found\n", rollNo);
        return;
    }

    CourseNode* course = createCourse(courseCode, marks);

    if (!student->coursesHead) {
        student->coursesHead = course;
    } else {
        CourseNode* current = student->coursesHead;
        while (current->next) {
            current = current->next;
        }
        current->next = course;
        course->prev = current;
    }
}

void addCourse(StudentDatabase* db, int rollNo, int courseCode, int marks) {
    if (rollNo <= 0) {
        printf("Please enter a valid roll no to add the course\n");
        return;
    }
    if (courseCode <= 0) {
        printf("Please enter a valid course code to add the course\n");
        return;
    }
    if (marks < 0) {
        printf("Please enter valid marks to add the course\n");
        return;
    }

    StudentNode* student = db->head;
    while (student && student->rollNo != rollNo) {
        student = student->next;
    }

    if (student) {
        CourseNode* course = createCourse(courseCode, marks);

        if (!student->coursesHead) {
            student->coursesHead = course;
        } else {
            CourseNode* current = student->coursesHead;
            while (current->next) {
                current = current->next;
            }
            current->next = course;
            course->prev = current;
        }
        student->numSubjects += 1;
    } else {
        printf("Failed to add course as no student exists with roll no %d.\n", rollNo);
    }
}


void modifyCourse(StudentDatabase* db, int rollNo, int courseCode, int newMarks) {
    if (rollNo <= 0) {
        printf("Please enter a valid roll no to modify the course\n");
        return;
    }
    if (courseCode <= 0) {
        printf("Please enter a valid course code to modify the course\n");
        return;
    }
    if (newMarks < 0) {
        printf("Please enter valid marks to modify the course\n");
        return;
    }
    StudentNode* student = db->head;
    while (student && student->rollNo != rollNo) {
        student = student->next;
    }
    if (student) {
        CourseNode* course = student->coursesHead;
        while (course && course->courseCode != courseCode) {
            course = course->next;
        }
        if (course) {
            course->marks = newMarks;
        } else {
            printf("Failed to modify the course with course code %d no such course exists. Please consider adding the course first.\n", courseCode);
        }
    } else {
        printf("Failed to modify the course for student with roll no: %d as no such student exists. Please consider adding the student first.\n", rollNo);
    }
}

void deleteCourse(StudentDatabase* db, int rollNo, int courseCode) {
    if (rollNo <= 0) {
        printf("Please enter a valid roll no to delete the course\n");
        return;
    }
    if (courseCode <= 0) {
        printf("Please enter a valid course code to delete the course\n");
        return;
    }
    StudentNode* student = db->head;
    while (student && student->rollNo != rollNo) {
        student = student->next;
    }
    if (student) {
        CourseNode* course = student->coursesHead;
        while (course && course->courseCode != courseCode) {
            course = course->next;
        }
        if (course) {
            if (course->prev) {
                course->prev->next = course->next;
            }
            if (course->next) {
                course->next->prev = course->prev;
            }
            if (course == student->coursesHead) {
                student->coursesHead = course->next;
            }
            student->numSubjects -= 1;
            free(course);
        } else {
            printf("Failed to delete the course with course code %d as no such course exists.\n", courseCode);
        }
    } else {
        printf("Failed to delete the course as no student exists with roll no %d.\n", rollNo);
    }
}
