/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

FILE* OpenFile(char *fname, char *mode)
{
    FILE* f = fopen(fname, mode);
    if (!f) {
        printf("Eroare deschidere fisier!\n");
        return NULL;
    }

    return f;
}

int InitTries(TNode *root1, TNode* root2)
{
    *root1 = InitNode();
    if (!(*root1))
        return 0;

    *root2 = InitNode();
    if (!(*root2)) {
        free((*root1)->child);
        free(*root1);
        return 0;
    }

    return 1;
}

int TaskCaller(TNode books, TNode authors, char *op, char *extradata, FILE *f)
{
    if (op[strlen(op) - 1] == '\n')
        op[strlen(op) - 1] = '\0';

    if (strcmp(op, "add_book") == 0) {
        if (AddToTries(books, authors, extradata) == 0)
            return 0;

    }

    if (strcmp(op, "search_book") == 0) {
        if (extradata[strlen(extradata) - 1] == '\n')
            extradata[strlen(extradata) - 1] = '\0';

        if (extradata[strlen(extradata) - 1] == '~')
            AutoCompleteBook(books, extradata, f);

        else
            FindBook(books, extradata, f);
    }

    if (strcmp(op, "list_author") == 0) {
        if (extradata[strlen(extradata) - 1] == '\n')
            extradata[strlen(extradata) - 1] = '\0';

        if (extradata[strlen(extradata) - 1] == '~')
            AutoCompleteAuthor(authors, extradata, f);
        else
            FindAuthor(authors, extradata, f);
    }

    if (strcmp(op, "search_by_author") == 0) {
        char *author = strtok(extradata, ":");
        if (author[strlen(author) - 1] == '\n')
            author[strlen(author) - 1] = '\0';

        if (author[strlen(author) - 1] == '~')
            AutoCompleteAuthor(authors, author, f);

        else {
            char *nume_carte = strtok(NULL, ":");
            FindBookByAuthor(authors, author, nume_carte, f);
        }
    }

    if (strcmp(op, "delete_book") == 0)
        DeleteBook(books, authors, extradata, f);

    return 1;
}

int main(int argc, char **argv)
{
    if (argc == 0) {
        printf("Functia nu a primit argumente!\n");
        return -1;
    }

    FILE *fin, *fout;
    fin = OpenFile(argv[1], "r");
    if (!fin)
        return -1;
    fout = OpenFile(argv[2], "w");
    if (!fout) {
        fclose(fin);
        return -1;
    }

    TNode books, authors;
    if (InitTries(&books, &authors) == 0)
        goto FileClose;

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fin) != - 1) {
        char *op = strtok(line, " ");
        if (TaskCaller(books, authors, op, strtok(NULL, ""), fout) == 0)
            break;
    }

    free(line);
    FreeTrieCarti(&books);
    FreeAuthorTrie(&authors);
    FileClose:
    fclose(fin);
    fclose(fout);

    return 0;
}