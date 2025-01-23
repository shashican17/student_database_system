#include <stdio.h>
#include "database.h"
#include "student.h"
#include "course.h"
#include "sDB.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    StudentDatabase* db = createDatabase();
    const char* inputFile = argv[1];
    const char* outputFile = "2127.out";

    if (parseInput(db, inputFile) != 0) {
        printf("Error processing input file: %s\n", inputFile);
        freeDatabase(db);
        return 1;
    }

    if (storeOutput(db, outputFile) != 0) {
        printf("Error writing to output file: %s\n", outputFile);
        freeDatabase(db);
        return 1;
    }

    freeDatabase(db);
    return 0;
}
