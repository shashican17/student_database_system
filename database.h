#ifndef DATABASE
#define DATABASE
#include "student.h"

struct StudentDatabase {
    StudentNode* head;
};

StudentDatabase* createDatabase(void);
int storeOutput(StudentDatabase* db, const char* filename);
int parseInput(StudentDatabase* db, const char* filename);
void freeDatabase(StudentDatabase* db);

#endif
