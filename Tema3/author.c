/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

/* functie care afiseaza titlul unei carti memorate la sf. unei cai */
void PrintTitle(void *info, FILE* f)
{
    TBook *book = (TBook *)info;
    fprintf(f, "%s\n", book->title);
}

void FindAuthor(TNode root, char *author, FILE *f)
{
    TNode last = Search(root, author);
    if (!last) {
        fprintf(f, "Autorul %s nu face parte din recomandarile tale.\n",
        author);
    return;
    }

    /* nu am verificat in acelasi if, ptc. last poate sa fie NULL si nu
    am cum sa accesez un camp de al lui */

    if (last->end == false) {
        fprintf(f, "Autorul %s nu face parte din recomandarile tale.\n",
        author);
        return;
    }

    TNode tbooks = (TNode)(last->info);

    int i;
    for (i = 0; i < NR_CH; i++)
        if (tbooks->child[i] != NULL)
            PrintTrieEnd(tbooks->child[i], f, PrintTitle);
}

void FindName(TNode root, FILE* f, int *ok)
{
    if (!root) /* safeguard */
        return;

    if (*ok) /* daca am gasit deja nume */
        return;

    /* daca am ajuns la finalul unei cai, extrag numele autorului */
    if (root->end == true) {
        TBook *book = (TBook *)(root->info);
        fprintf(f, "%s\n", book->author);
        (*ok) = 1; /* daca ok = 1, nu mai iterez */
    }

    int i;
    for (i = 0; i < NR_CH; i++) {
        if (*ok) /* daca am gasit deja nume, nu mai iterez */
            break;

        if (root->child[i] != NULL)
            FindName(root->child[i], f, ok);
    }
}

void CompleteName(TNode root, int *counter, FILE* f)
{
    if (*counter == 3) /* daca am afisat deja 3 nume, ma opresc */
        return;

    if (root->end == true) {
        int ok = 0;
        FindName((TNode)(root->info), f, &ok);
        (*counter)++;
    }

    int i;
    for (i = 0; i < NR_CH; i++) {
        if (*counter == 3)
            break;
        if (root->child[i] != NULL)
            CompleteName(root->child[i], counter, f);
    }
}

void AutoCompleteAuthor(TNode root, char *prefix, FILE *f)
{
    prefix[strlen(prefix) - 1] = '\0'; /* elimin ~ */
    TNode last = Search(root, prefix);

    if (!last) {
        fprintf(f, "Niciun autor gasit.\n");
        return;
    }

    int counter = 0, i;

    /* daca prefixul este deja un nume de autor */
    if (last->end == true) {
        fprintf(f, "%s\n", prefix);
        counter++;
    }

    for (i = 0; i < NR_CH && counter < 3; i++)
        if (last->child[i] != NULL)
            CompleteName(last->child[i], &counter, f);
}

void FreeAuthorTrie(TNode *root)
{
    if (!(*root))
        return;

    int i;
    for (i = 0; i < NR_CH; i++)
        if ((*root)->child[i])
            FreeAuthorTrie(&((*root)->child[i]));

    if ((*root)->end) {
        TNode node = (TNode)((*root)->info);
        FreeTrie(&node);
    }

    free((*root)->child);
    free(*root);
    *root = NULL;
}