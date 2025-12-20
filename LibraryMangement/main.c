#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "book.h"
#include "borrow.h"
#include "fileops.h"
#include "utils.h"

void printHeader() {
    printf("\n=====================================================\n");
    printf("         CLI Library Management System   \n");
    printf("=====================================================\n");
}

void menu() {
    printf("\nMenu:\n");
    printf(" 1  Register Student\n");
    printf(" 2  Register Book\n");
    printf(" 3  Borrow Book\n");
    printf(" 4  Return Book\n");
    printf(" 5  Update Student\n");
    printf(" 6  Update Book\n");
    printf(" 7  Remove Student\n");
    printf(" 8  Remove Book\n");
    printf(" 9  Search Student by Name\n");
    printf("10  Search Book by Name\n");
    printf("11  List All Books\n");
    printf("12  List All Borrowed Books\n");
    printf("13  List Overdue Books\n");
    printf("14  List All Students\n");
    printf(" 0  Exit\n");
}

int main() {
    int choice;
    char buf[128];

    loadAllData(); /* this prints [INFO] messages as requested */

    while(1) {
        printHeader();
        menu();
        choice = getInt("Choose an option: ", 0, 100);

        switch(choice) {
            case 1: registerStudent(); break;
            case 2: registerBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: updateStudent(); break;
            case 6: updateBook(); break;
            case 7: removeStudent(); break;
            case 8: removeBook(); break;
            case 9: {
                getString("Enter student name to search: ", buf, sizeof(buf));
                int idx = searchStudentByName(buf);
                if(idx == -1) printf("[INFO] Student not found.\n");
                else printf("[FOUND] %s | Roll: %d | Program: %s | Card: %s\n",
                            students[idx].name, students[idx].rollNumber, students[idx].program, students[idx].libraryCard);
                break;
            }
            case 10: {
                getString("Enter book name to search: ", buf, sizeof(buf));
                int idx = searchBookByName(buf);
                if(idx == -1) printf("[INFO] Book not found.\n");
                else printf("[FOUND] ID: %d | %s | Author: %s | Ed: %s | Total: %d | Borrowed: %d\n",
                            books[idx].id, books[idx].name, books[idx].author, books[idx].edition,
                            books[idx].totalCopies, books[idx].borrowedCopies);
                break;
            }
            case 11: listAllBooks(); break;
            case 12: listAllBorrowedBooks(); break;
            case 13: listOverdueBooks(); break;
            case 14: listAllStudents(); break;
            case 0:
                saveAllData();
                printf("[OK] All data saved. Exiting...\n");
                exit(0);
            default:
                printf("[ERROR] Invalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        fgets(buf, sizeof(buf), stdin);
    }

    return 0;
}

