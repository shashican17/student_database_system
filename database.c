#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "course.h"
#include "student.h"
#include "sDB.h"


StudentDatabase* createDatabase(void) {
    StudentDatabase* db = (StudentDatabase*)malloc(sizeof(StudentDatabase));
    if (db == NULL) {
        perror("Failed to allocate memory for database");
        exit(1);
    }
    db->head = NULL;
    return db;
}


int storeOutput(StudentDatabase* db, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Unable to open output file: %s\n", filename);
        perror("fopen");
        return -1;
    }

    StudentNode* student = db->head;
    while (student) {
        int bytes_written = fprintf(file, "%d, %s, %.1f, %d\n", student->rollNo, student->name, student->cgpa, student->numSubjects);
        if (bytes_written < 0) {
            fprintf(stderr, "Error writing to output file: %s\n", filename);
            perror("fprintf");
            fclose(file);
            return -1;
        }

        CourseNode* course = student->coursesHead;
        while (course) {
            bytes_written = fprintf(file, "%d, %d\n", course->courseCode, course->marks);
            if (bytes_written < 0) {
                fprintf(stderr, "Error writing to output file: %s\n", filename);
                perror("fprintf");
                fclose(file);
                return -1;
            }
            course = course->next;
        }
        student = student->next;
    }

    fflush(file);
    int result = fclose(file);
    if (result != 0) {
        fprintf(stderr, "Error closing output file: %s\n", filename);
        perror("fclose");
        return -1;
    }

    return 0;
}

int parseInput(StudentDatabase* studentDB, const char* inputFileName) {
    FILE* file = fopen(inputFileName, "r");
    if (!file) {
        perror("Unable to create/open input file");
        return -1;
    }

    char currentLine[256];
    char currentCommand[50] = "initial";
    int currentStudentRollNo = -1;

    while (fgets(currentLine, sizeof(currentLine), file)) {
        char* trimmedLine = strtok(currentLine, "\n");

        if (!trimmedLine || trimmedLine[0] == '#') {
            if (trimmedLine && strstr(trimmedLine, "initial database")) {
                strcpy(currentCommand, "initial");
            } 
            else if (trimmedLine && strstr(trimmedLine, "add student")) {
                strcpy(currentCommand, "addStudent");
            } 
            else if (trimmedLine && strstr(trimmedLine, "add course")) {
                strcpy(currentCommand, "addCourse");
            } 
            else if (trimmedLine && strstr(trimmedLine, "modify student")) {
                strcpy(currentCommand, "modifyStudent");
            } 
            else if (trimmedLine && strstr(trimmedLine, "modify course")) {
                strcpy(currentCommand, "modifyCourse");
            } 
            else if (trimmedLine && strstr(trimmedLine, "delete student")) {
                strcpy(currentCommand, "deleteStudent");
            } 
            else if (trimmedLine && strstr(trimmedLine, "delete course")) {
                strcpy(currentCommand, "deleteCourse");
            }
            continue;
        }

        char* lineTokens[4];
        int tokenCount = 0;
        char* token = strtok(trimmedLine, ", ");
        while (token) {
            lineTokens[tokenCount++] = token;
            token = strtok(NULL, ", ");
        }

        if (!strcmp(currentCommand, "initial")) {
            if (tokenCount == 4) {
                int studentRollNo = atoi(lineTokens[0]);
                currentStudentRollNo = studentRollNo;
                char* studentName = lineTokens[1];
                float studentCGPA = atof(lineTokens[2]);
                int totalSubjects = atoi(lineTokens[3]);
                addStudent(studentDB, studentRollNo, studentName, studentCGPA, totalSubjects);
            } else if (tokenCount == 2 && currentStudentRollNo != -1) {  
                int courseCode = atoi(lineTokens[0]);
                int courseMarks = atoi(lineTokens[1]);
                addCourseInitial(studentDB, currentStudentRollNo, courseCode, courseMarks);
            } else {
                printf("Invalid input encountered\n");
            }
        }  
        else if (strcmp(currentCommand, "addStudent") == 0 && tokenCount == 4) {
            int studentRollNo = atoi(lineTokens[0]);
            char* studentName = lineTokens[1];
            float studentCGPA = atof(lineTokens[2]);
            int totalSubjects = atoi(lineTokens[3]);
            addStudent(studentDB, studentRollNo, studentName, studentCGPA, totalSubjects);
        } 
        else if (strcmp(currentCommand, "addCourse") == 0 && tokenCount == 3) {
            int studentRollNo = atoi(lineTokens[0]);
            int courseCode = atoi(lineTokens[1]);
            int courseMarks = atoi(lineTokens[2]);
            addCourse(studentDB, studentRollNo, courseCode, courseMarks);
        } 
        else if (strcmp(currentCommand, "modifyStudent") == 0 && tokenCount == 2) {
            int studentRollNo = atoi(lineTokens[0]);
            float newCGPA = atof(lineTokens[1]);
            modifyStudent(studentDB, studentRollNo, newCGPA);
        } 
        else if (strcmp(currentCommand, "modifyCourse") == 0 && tokenCount == 3) {
            int studentRollNo = atoi(lineTokens[0]);
            int courseCode = atoi(lineTokens[1]);
            int newMarks = atoi(lineTokens[2]);
            modifyCourse(studentDB, studentRollNo, courseCode, newMarks);
        } 
        else if (strcmp(currentCommand, "deleteStudent") == 0 && tokenCount == 1) {
            int studentRollNo = atoi(lineTokens[0]);
            deleteStudent(studentDB, studentRollNo);
        } 
        else if (strcmp(currentCommand, "deleteCourse") == 0 && tokenCount == 2) {
            int studentRollNo = atoi(lineTokens[0]);
            int courseCode = atoi(lineTokens[1]);
            deleteCourse(studentDB, studentRollNo, courseCode);
        } else {
            printf("Invalid input encountered\n");
        }
    }
    fclose(file);
    return 0;
}



void freeDatabase(StudentDatabase* db) {
    if (db == NULL) return;

    StudentNode* current = db->head;
    while (current != NULL) {
        StudentNode* temp = current;
        current = current->next;

        CourseNode* currentCourse = temp->coursesHead;
        while (currentCourse != NULL) {
            CourseNode* tempCourse = currentCourse;
            currentCourse = currentCourse->next;
            free(tempCourse);
        }

        free(temp);
    }

    free(db);
}