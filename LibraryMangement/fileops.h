#ifndef FILEOPS_H
#define FILEOPS_H

#include "student.h"
#include "book.h"
#include "borrow.h"

/* File operations */
void loadStudents();
void saveStudents();
void loadBooks();
void saveBooks();
void loadBorrows();
void saveBorrows();

/* Convenience */
void saveAllData();
void loadAllData();

#endif

