#ifndef BORROW_H
#define BORROW_H

#include <time.h>
#include "student.h"
#include "book.h"

#define MAX_BORROWS 500

typedef struct {
    int bookId;
    int studentRoll;
    time_t borrowDate;   /* stored as time_t */
    float fine;          /* stored fine (optional) */
} BorrowRecord;

extern BorrowRecord borrowRecords[MAX_BORROWS];
extern int borrowCount;

/* Borrow functions */
void borrowBook();
void returnBook();
void listAllBorrowedBooks();
void listOverdueBooks();

#endif

