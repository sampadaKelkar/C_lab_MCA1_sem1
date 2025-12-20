#include <stdio.h>
#include <string.h>
#include <time.h>
#include "borrow.h"
#include "utils.h"
#include "fileops.h"

BorrowRecord borrowRecords[MAX_BORROWS];
int borrowCount = 0;

#define ALLOWED_DAYS 14
#define FINE_PER_DAY 10.0f

/* returns days between borrowDate and now, or -9999 on invalid */
int daysSince(time_t borrowTime) {
    if(borrowTime == (time_t)-1) return -9999;
    time_t now = time(NULL);
    double diff = difftime(now, borrowTime);
    return (int)(diff / (60.0 * 60.0 * 24.0));
}

void borrowBook() {
    if(borrowCount >= MAX_BORROWS) {
        printf("[ERROR] Borrow limit reached (%d)\n", MAX_BORROWS);
        return;
    }

    int roll = getInt("Enter student roll number: ", 1, 999999);
    int sIndex = -1;
    int i, j;
    for(i = 0; i < studentCount; i++) {
        if(students[i].rollNumber == roll) {
            sIndex = i;
            break;
        }
    }
    if(sIndex == -1) {
        printf("[ERROR] Student not found!\n");
        return;
    }

    /* Block borrowing if student has overdue books */
    for(i = 0; i < borrowCount; i++) {
        if(borrowRecords[i].studentRoll == roll) {
            int ds = daysSince(borrowRecords[i].borrowDate);
            if(ds != -9999 && ds > ALLOWED_DAYS) {
                printf("[BLOCK] Student has overdue book(s). Clear fines before borrowing.\n");
                return;
            }
        }
    }

    int id = getInt("Enter book ID: ", 1, 999999);
    int bIndex = -1;
    for(i = 0; i < bookCount; i++) {
        if(books[i].id == id) {
            bIndex = i;
            break;
        }
    }
    if(bIndex == -1) {
        printf("[ERROR] Book not found!\n");
        return;
    }

    if(books[bIndex].borrowedCopies >= books[bIndex].totalCopies) {
        printf("[ERROR] No copies available!\n");
        return;
    }

    for(i = 0; i < borrowCount; i++) {
        if(borrowRecords[i].bookId == id && borrowRecords[i].studentRoll == roll) {
            printf("[ERROR] Student already borrowed this book!\n");
            return;
        }
    }

    BorrowRecord br;
    br.bookId = id;
    br.studentRoll = roll;
    br.fine = 0.0f;

    /* ask user for borrow date; if empty -> use today */
    {
        char dateBuf[32];
        getString("Enter borrow date (YYYY-MM-DD) or press Enter for today: ", dateBuf, sizeof(dateBuf));
        if(dateBuf[0] == '\0') {
            br.borrowDate = time(NULL);
        } else {
            time_t parsed = parseDateString(dateBuf);
            if(parsed == (time_t)-1) {
                printf("[ERROR] Invalid date format. Use YYYY-MM-DD\n");
                return;
            }
            br.borrowDate = parsed;
        }
    }

    borrowRecords[borrowCount++] = br;
    books[bIndex].borrowedCopies++;
    saveAllData(); /* autosave */
    printf("[OK] Book borrowed successfully by %s (Roll: %d).\n", students[sIndex].name, roll);
}

void returnBook() {
    int roll = getInt("Enter student roll number: ", 1, 999999);
    int id = getInt("Enter book ID: ", 1, 999999);

    int index = -1;
    int i, j;
    for(i = 0; i < borrowCount; i++) {
        if(borrowRecords[i].studentRoll == roll && borrowRecords[i].bookId == id) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        printf("[ERROR] Borrow record not found!\n");
        return;
    }

    /* compute overdue and fine (display here or in overdue list only as per your choice).
       Style 1: fines shown only in overdue check - but also inform user on return. */
    {
        int days = daysSince(borrowRecords[index].borrowDate);
        if(days == -9999) {
            printf("[WARN] Borrow date invalid, cannot compute fine.\n");
        } else {
            int overdue = days - ALLOWED_DAYS;
            if(overdue > 0) {
                float fine = overdue * FINE_PER_DAY;
                printf("[FINE] Book is overdue by %d days. Fine = %.2f\n", overdue, fine);
            } else {
                printf("[OK] Book returned on time. No fine.\n");
            }
        }
    }

    /* decrease borrowedCopies safely */
    for(i = 0; i < bookCount; i++) {
        if(books[i].id == id) {
            if(books[i].borrowedCopies > 0) books[i].borrowedCopies--;
            break;
        }
    }

    /* remove borrow record */
    for(i = index; i < borrowCount - 1; i++) {
        borrowRecords[i] = borrowRecords[i + 1];
    }
    borrowCount--;

    saveAllData(); /* autosave */
    printf("[OK] Book returned successfully.\n");
}

void listAllBorrowedBooks() {
    int i, j;
    char dateStr[16];
    printf("\n+------------------ Borrowed Books ------------------+\n");
    if(borrowCount == 0) {
        printf("| No borrowed books currently.\n");
    } else {
        for(i = 0; i < borrowCount; i++) {
            int sIndex = -1, bIndex = -1;
            for(j = 0; j < studentCount; j++) if(students[j].rollNumber == borrowRecords[i].studentRoll) sIndex = j;
            for(j = 0; j < bookCount; j++) if(books[j].id == borrowRecords[i].bookId) bIndex = j;
            if(sIndex != -1 && bIndex != -1) {
                formatDate(borrowRecords[i].borrowDate, dateStr, sizeof(dateStr));
                printf("| Student: %-20s | Roll: %6d | Book: %-25s | Date: %s\n",
                       students[sIndex].name,
                       students[sIndex].rollNumber,
                       books[bIndex].name,
                       dateStr);
            }
        }
    }
    printf("+----------------------------------------------------+\n");
}

void listOverdueBooks() {
    int i, j;
    int found = 0;
    char dateStr[16];
    printf("\n+------------------ Overdue Books -------------------+\n");
    for(i = 0; i < borrowCount; i++) {
        int sIndex = -1, bIndex = -1;
        for(j = 0; j < studentCount; j++) if(students[j].rollNumber == borrowRecords[i].studentRoll) sIndex = j;
        for(j = 0; j < bookCount; j++) if(books[j].id == borrowRecords[i].bookId) bIndex = j;
        if(sIndex != -1 && bIndex != -1) {
            int days = daysSince(borrowRecords[i].borrowDate);
            if(days != -9999) {
                int overdue = days - ALLOWED_DAYS;
                if(overdue > 0) {
                    found = 1;
                    float fine = overdue * FINE_PER_DAY;
                    formatDate(borrowRecords[i].borrowDate, dateStr, sizeof(dateStr));
                    printf("| Student: %-18s | Roll: %6d | Book: %-20s | Date: %s | Days: %3d | OVERDUE %3d | Fine: %.2f\n",
                           students[sIndex].name, students[sIndex].rollNumber,
                           books[bIndex].name, dateStr, days, overdue, fine);
                }
            }
        }
    }
    if(!found) printf("| No overdue books.\n");
    printf("+----------------------------------------------------+\n");
}

