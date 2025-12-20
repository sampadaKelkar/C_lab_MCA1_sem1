#include <stdio.h>
#include <string.h>
#include "student.h"
#include "utils.h"
#include "fileops.h"

Student students[MAX_STUDENTS];
int studentCount = 0;

void registerStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf("[ERROR] Student limit reached (%d)\n", MAX_STUDENTS);
        return;
    }

    Student s;
    int i;

    s.rollNumber = getInt("Enter roll number: ", 1, 999999);

    /* check duplicate roll */
    for(i = 0; i < studentCount; i++) {
        if(students[i].rollNumber == s.rollNumber) {
            printf("[ERROR] Roll number already exists.\n");
            return;
        }
    }

    getString("Enter student name: ", s.name, sizeof(s.name));
    if(!validateNonEmptyString(s.name)) {
        printf("[ERROR] Name cannot be empty.\n");
        return;
    }

    getString("Enter program: ", s.program, sizeof(s.program));
    getString("Enter library card number: ", s.libraryCard, sizeof(s.libraryCard));

    students[studentCount++] = s;
    saveAllData(); /* autosave */
    printf("[OK] Student registered successfully. Total students: %d\n", studentCount);
}

int searchStudentByName(char *name) {
    int i;
    for(i = 0; i < studentCount; i++) {
        if(strcmp(students[i].name, name) == 0) return i;
    }
    return -1;
}

int searchStudentByRoll(int rollNumber) {
    int i;
    for(i = 0; i < studentCount; i++) {
        if(students[i].rollNumber == rollNumber) return i;
    }
    return -1;
}

void updateStudent() {
    int roll = getInt("Enter roll number to update: ", 1, 999999);
    int index = searchStudentByRoll(roll);
    if(index == -1) {
        printf("[ERROR] Student not found!\n");
        return;
    }
    getString("Enter new name: ", students[index].name, sizeof(students[index].name));
    getString("Enter new program: ", students[index].program, sizeof(students[index].program));
    getString("Enter new library card number: ", students[index].libraryCard, sizeof(students[index].libraryCard));
    saveAllData();
    printf("[OK] Student updated successfully.\n");
}

void removeStudent() {
    int roll = getInt("Enter roll number to remove: ", 1, 999999);
    int index = searchStudentByRoll(roll);
    int i;
    if(index == -1) {
        printf("[ERROR] Student not found!\n");
        return;
    }

    for(i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    saveAllData();
    printf("[OK] Student removed successfully. Remaining: %d\n", studentCount);
}

void listAllStudents() {
    int i;
    printf("\n+------------------- Registered Students -------------------+\n");
    if(studentCount == 0) {
        printf("| No students registered.\n");
    } else {
        for(i = 0; i < studentCount; i++) {
            printf("| Roll: %6d | Name: %-20s | Program: %-15s | Card: %-15s\n",
                   students[i].rollNumber,
                   students[i].name,
                   students[i].program,
                   students[i].libraryCard);
        }
    }
    printf("+-----------------------------------------------------------+\n");
}


