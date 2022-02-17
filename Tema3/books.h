/* Dumitru Radu-Andrei 312CB */

#ifndef BOOK_H
#define BOOK_H

typedef struct Book {
    char *title;
    char *author;
    int rating;
    int pages;
} TBook;

/* functii implementate */
void FindBook(TNode root, char *titlu, FILE *f); /* cauta o carte in primul trie */
void AutoCompleteBook(TNode root, char *prefix, FILE *f); /* cauta o carte dupa prefix in primul trie */
void FindBookByAuthor(TNode root, char *author, char *titlu, FILE *f); /* cauta o carte in trie-ul coresp. autorului sau */
void DeleteBook(TNode fir_root, TNode sec_root, char *titlu, FILE *f); /* sterge o carte din trie-uri */
void FreeTrieCarti(TNode *root); /* elibereaza memoria alocata pentru trie-ul unde se retin cartile */

#endif