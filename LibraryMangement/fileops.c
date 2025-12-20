#include <stdio.h>
#include <string.h>
#include "fileops.h"
#include "student.h"
#include "book.h"
#include "borrow.h"

/* filenames */
#define STUDENT_FILE "students.dat"
#define BOOK_FILE    "books.dat"
#define BORROW_FILE  "borrow.dat"

extern Student students[];
extern int studentCount;
extern Book books[];
extern int bookCount;
extern BorrowRecord borrowRecords[];
extern int borrowCount;

void loadStudents() {
    FILE *f = fopen(STUDENT_FILE, "rb");
    if(!f) {
        printf("[INFO] %s not found. Starting with 0 students.\n", STUDENT_FILE);
        studentCount = 0;
        return;
    }
    if(fread(&studentCount, sizeof(int), 1, f) != 1) {
        studentCount = 0;
        fclose(f);
        return;
    }
    if(studentCount > MAX_STUDENTS) studentCount = MAX_STUDENTS;
    fread(students, sizeof(Student), studentCount, f);
    fclose(f);
    printf("[INFO] Loaded %d students.\n", studentCount);
}

void saveStudents() {
    FILE *f = fopen(STUDENT_FILE, "wb");
    if(!f) {
        printf("[ERROR] Unable to write %s\n", STUDENT_FILE);
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, f);
    if(studentCount > 0) fwrite(students, sizeof(Student), studentCount, f);
    fclose(f);
}

void loadBooks() {
    FILE *f = fopen(BOOK_FILE, "rb");
    if(!f) {
        printf("[INFO] %s not found. Starting with 0 books.\n", BOOK_FILE);
        bookCount = 0;
        return;
    }
    if(fread(&bookCount, sizeof(int), 1, f) != 1) {
        bookCount = 0;
        fclose(f);
        return;
    }
    if(bookCount > MAX_BOOKS) bookCount = MAX_BOOKS;
    fread(books, sizeof(Book), bookCount, f);
    fclose(f);
    printf("[INFO] Loaded %d books.\n", bookCount);
}

void saveBooks() {
    FILE *f = fopen(BOOK_FILE, "wb");
    if(!f) {
        printf("[ERROR] Unable to write %s\n", BOOK_FILE);
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, f);
    if(bookCount > 0) fwrite(books, sizeof(Book), bookCount, f);
    fclose(f);
}

void loadBorrows() {
    FILE *f = fopen(BORROW_FILE, "rb");
    if(!f) {
        printf("[INFO] %s not found. Starting with 0 borrow records.\n", BORROW_FILE);
        borrowCount = 0;
        return;
    }
    if(fread(&borrowCount, sizeof(int), 1, f) != 1) {
        borrowCount = 0;
        fclose(f);
        return;
    }
    if(borrowCount > MAX_BORROWS) borrowCount = MAX_BORROWS;
    fread(borrowRecords, sizeof(BorrowRecord), borrowCount, f);
    fclose(f);
    printf("[INFO] Loaded %d borrow records.\n", borrowCount);
}

void saveBorrows() {
    FILE *f = fopen(BORROW_FILE, "wb");
    if(!f) {
        printf("[ERROR] Unable to write %s\n", BORROW_FILE);
        return;
    }
    fwrite(&borrowCount, sizeof(int), 1, f);
    if(borrowCount > 0) fwrite(borrowRecords, sizeof(BorrowRecord), borrowCount, f);
    fclose(f);
}

void saveAllData() {
    saveStudents();
    saveBooks();
    saveBorrows();
}

void loadAllData() {
    loadStudents();
    loadBooks();
    loadBorrows();
}

