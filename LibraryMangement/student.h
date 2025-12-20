#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100

typedef struct {
    int rollNumber;
    char name[50];
    char program[50];
    char libraryCard[20];
} Student;

extern Student students[MAX_STUDENTS];
extern int studentCount;

/* Student functions */
void registerStudent();
int searchStudentByName(char *name);
int searchStudentByRoll(int rollNumber);
void updateStudent();
void removeStudent();
void listAllStudents();

#endif

