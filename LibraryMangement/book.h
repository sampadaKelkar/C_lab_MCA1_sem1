#ifndef BOOK_H
#define BOOK_H

#define MAX_BOOKS 200

typedef struct {
    int id;
    char name[100];
    char author[50];
    char edition[20];
    int totalCopies;
    int borrowedCopies;
} Book;

extern Book books[MAX_BOOKS];
extern int bookCount;

/* Book functions */
void registerBook();
int searchBookByName(char *name);
int searchBookByAuthor(char *author);
void updateBook();
void removeBook();
void listAllBooks();

#endif

