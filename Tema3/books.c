/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

/* aloca memorie si prelucreaza datele pentru o carte */
TBook *InitBook(char *book_data)
{
    TBook *book = malloc(sizeof(TBook));
    if (!book) {
        printf("Eroare alocare structura book!\n");
        return NULL;
    }

    book->title = strdup(strtok(book_data, ":"));
    if (!book->title) {
        printf("Eroare strdup title!\n");
        free(book);
        return NULL;
    }

    book->author = strdup(strtok(NULL, ":"));
    if (!book->author) {
        printf("Eroare strdup author!\n");
        free(book->title);
        free(book);
        return NULL;
    }

    book->rating = atoi(strtok(NULL, ":"));
    book->pages = atoi(strtok(NULL, ":"));

    return (void *)book;
}

/* elibereaza memoria alocata pentru o carte */
void FreeBook(TBook **book)
{
    free((*book)->author);
    free((*book)->title);
    free(*book);
    *book = NULL;
}

int AddToTries (TNode fir, TNode sec, char *book_data)
{
    TBook *book = InitBook(book_data);
    if (!book)
        return 0;

    TNode last = Insert(fir, book->title);
    if (!last) {
        FreeBook(&book);
        return 0;
    }

    if (last->info == NULL)
        last->info = book;
    else {
        FreeBook(&book);
        return 1;
    }

    /* nu mai eliberez memoria alocata pentru carte, in caz de fail,
    ptc aceasta se afla in primul trie, si va fi eliberata odata cu acesta */

    last = Insert(sec, book->author);
    if (!last)
        return 0;

    /* daca autorul nu era in trie, creez un nou trie pt. a retine
    cartile scrise de acesta */
    TNode root;
    if (last->info == NULL) {
        root = InitNode();
        if (!root)
            return 0;
    }

    else
        root = (TNode)(last->info);

    /* introduc cartea in trie-ul coresp. autorului */
    TNode last_T3 = Insert(root, book->title);

    if (!last_T3)
        return 0;

    last_T3->info = book;
    last->info = root;

    return 1;
}

void FindBook(TNode root, char *title, FILE *f)
{
    TNode found = Search(root, title);

    if (!found) {
        fprintf(f, "Cartea %s nu exista in recomandarile tale.\n", title);
        return;
    }

    if (found->end == true) {
        TBook *book = (TBook *)(found->info);
        fprintf(f, "Informatii recomandare: %s, %s, %d, %d\n",
        book->title, book->author, book->rating, book->pages);
        return;
    }


    fprintf(f, "Cartea %s nu exista in recomandarile tale.\n", title);
}

void CompleteTitle(TNode root, int *counter, FILE *f)
{
    if (*counter == 3)
        return;

    if (root->end == true) {
        TBook *book = (TBook *)(root->info);
        fprintf(f, "%s\n", book->title);
        (*counter)++;
    }

    int i;
    for (i = 0; i < NR_CH; i++)
        if (root->child[i] != NULL) {
            if (*counter == 3)
                break;
            CompleteTitle(root->child[i], counter, f);
        }
}

void AutoCompleteBook(TNode root, char *prefix, FILE *f)
{
    prefix[strlen(prefix) - 1] = '\0';

    TNode last = Search(root, prefix);
    if (!last) {
        fprintf(f, "Nicio carte gasita.\n");
        return;
    }
    int i, counter = 0;

    /* daca prefixul dat este deja un titlu */
    if (last->end) {
        counter++;
        fprintf(f, "%s\n", prefix);
    }

    for (i = 0; i < NR_CH && counter < 3; i++) {
        if (last->child[i] != NULL) {
            CompleteTitle(last->child[i], &counter, f);
        }
    }
}

void FindBookByAuthor(TNode root, char *author, char *title, FILE *f)
{
    if (title[strlen(title) - 1] == '\n')
        title[strlen(title) - 1] = '\0';

    TNode last = Search(root, author);
    if (!last) {
        fprintf(f, "Autorul %s nu face parte din recomandarile tale.\n",
        author);
        return;
    }

    if (last->end == false) {
        fprintf(f, "Autorul %s nu face parte din recomandarile tale.\n",
        author);
        return;
    }

    if (title[strlen(title) - 1] == '~') {
        AutoCompleteBook((TNode)(last->info), title, f);
        return;
    }
    else
        FindBook((TNode)(last->info), title, f);

}

void DeleteBook(TNode fir_root, TNode sec_root, char *title, FILE *f)
{
    if (title[strlen(title) - 1] == '\n')
        title[strlen(title) - 1] = '\0';

    TNode last = Search(fir_root, title);

    if (!last) {
        fprintf(f, "Cartea %s nu exista in recomandarile tale.\n", title);
        return;
    }

    else if (last->end == false) {
        fprintf(f, "Cartea %s nu exista in recomandarile tale.\n", title);
        return;
    }

    TBook *book = (TBook *)(last->info);

    size_t c = 0; /* caracterul curent la care ma aflu in string */
    Delete(&fir_root, title, c);

    /* iau trie-ul cu cartile autorului si sterg cartea */
    last = Search(sec_root, book->author);
    if (!last) /* safeguard, daca am cartea sigur am si autorul */
        return;

    TNode thrd_root = (TNode)(last->info);
    c = 0;
    Delete(&thrd_root, title, c);

    c = 0;
    if (thrd_root == NULL) /* daca toate cartile autorului au fost sterse */
        Delete(&sec_root, book->author, c);

    free(book->title);
    free(book->author);
    free(book);
}

void FreeTrieCarti(TNode *root)
{
    if (!(*root))
        return;

    int i;
    for (i = 0; i < NR_CH; i++)
        if ((*root)->child[i])
            FreeTrieCarti(&((*root)->child[i]));

    if ((*root)->end) {
        TBook *book = (TBook *)(*root)->info;
        FreeBook(&book);
    }

    free((*root)->child);
    free(*root);
    (*root) = NULL;
}