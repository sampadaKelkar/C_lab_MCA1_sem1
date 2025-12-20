#include <stdio.h>
#include <string.h>
#include "book.h"
#include "utils.h"
#include "fileops.h"

Book books[MAX_BOOKS];
int bookCount = 0;

void registerBook() {
    if(bookCount >= MAX_BOOKS) {
        printf("[ERROR] Book limit reached (%d)\n", MAX_BOOKS);
        return;
    }

    Book b;
    int i;

    b.id = getInt("Enter book ID: ", 1, 999999);

    for(i = 0; i < bookCount; i++) {
        if(books[i].id == b.id) {
            printf("[ERROR] Book ID already exists.\n");
            return;
        }
    }

    getString("Enter book name: ", b.name, sizeof(b.name));
    if(!validateNonEmptyString(b.name)) {
        printf("[ERROR] Book name cannot be empty.\n");
        return;
    }
    getString("Enter author name: ", b.author, sizeof(b.author));
    getString("Enter edition: ", b.edition, sizeof(b.edition));
    b.totalCopies = getInt("Enter total copies: ", 1, 10000);
    b.borrowedCopies = 0;

    books[bookCount++] = b;
    saveAllData();
    printf("[OK] Book registered successfully. Total books: %d\n", bookCount);
}

int searchBookByName(char *name) {
    int i;
    for(i = 0; i < bookCount; i++) {
        if(strcmp(books[i].name, name) == 0) return i;
    }
    return -1;
}

int searchBookByAuthor(char *author) {
    int i;
    for(i = 0; i < bookCount; i++) {
        if(strcmp(books[i].author, author) == 0) return i;
    }
    return -1;
}

void updateBook() {
    int id = getInt("Enter book ID to update: ", 1, 999999);
    int index = -1;
    int i;
    for(i = 0; i < bookCount; i++) if(books[i].id == id) index = i;
    if(index == -1) {
        printf("[ERROR] Book not found!\n");
        return;
    }

    getString("Enter new name: ", books[index].name, sizeof(books[index].name));
    getString("Enter new author: ", books[index].author, sizeof(books[index].author));
    getString("Enter new edition: ", books[index].edition, sizeof(books[index].edition));
    books[index].totalCopies = getInt("Enter total copies: ", 1, 10000);
    if(books[index].borrowedCopies > books[index].totalCopies) {
        /* guard: borrowed cannot exceed total; if happens, clamp */
        books[index].borrowedCopies = books[index].totalCopies;
    }
    saveAllData();
    printf("[OK] Book updated successfully.\n");
}

void removeBook() {
    int id = getInt("Enter book ID to remove: ", 1, 999999);
    int index = -1;
    int i;
    for(i = 0; i < bookCount; i++) if(books[i].id == id) index = i;
    if(index == -1) {
        printf("[ERROR] Book not found!\n");
        return;
    }

    for(i = index; i < bookCount - 1; i++) books[i] = books[i + 1];
    bookCount--;
    saveAllData();
    printf("[OK] Book removed successfully. Remaining: %d\n", bookCount);
}

void listAllBooks() {
    int i;
    printf("\n+------------------- Library Books -------------------+\n");
    if(bookCount == 0) {
        printf("| No books registered.\n");
    } else {
        for(i = 0; i < bookCount; i++) {
            int available = books[i].totalCopies - books[i].borrowedCopies;
            if(available < 0) available = 0;
            printf("| ID: %d | %-30s | Author: %-15s | Ed: %-3s | Total: %3d | Borrowed: %3d | Avail: %3d\n",
                   books[i].id,
                   books[i].name,
                   books[i].author,
                   books[i].edition,
                   books[i].totalCopies,
                   books[i].borrowedCopies,
                   available);
        }
    }
    printf("+-----------------------------------------------------+\n");
}

