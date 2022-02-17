/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

TNode InitNode()
{
    TNode node = malloc(sizeof(Node));
    if (!node) {
        printf("Eroare alocare node!\n");
        return NULL;
    }

    node->child = calloc(NR_CH, sizeof(Node));
    if (!node->child) {
        printf("Eroare alocare vector copii!\n");
        free(node);
        return NULL;
    }

    node->info = NULL;
    node->end = false;

    return node;
}

/* functie care calculeaza cheia dupa ordinea lexicografica data */
int KeyFun(char chr)
{
    if (chr >= 'a' && chr <= 'z')
        return chr - ASCII_START_LOWCASE;

    if (chr >= 'A' && chr <= 'Z')
        return chr - ASCII_START_UPCASE + NR_LOWCASE;

    if (chr >= '0' && chr <= '9')
        return chr -'0' + NR_LOWCASE + NR_UPPCASE;

    size_t i;
    for (i = 0; i < strlen(SPECIAL_CH); i++)
        if (chr == SPECIAL_CH[i])
            return NR_LOWCASE + NR_UPPCASE + NR_DIGITS + i;

    return -1;
}
/* functie care insereaza intr-un trie */
TNode Insert(TNode root, char *string)
{
    TNode next = root;
    int ind;
    size_t i;
    for (i = 0; i < strlen(string); i++) {
        ind = KeyFun(string[i]);
        if (ind == -1) /* daca are un caracter care nu e in alfabetul dat, returnez NULL */
            return NULL;

        if (!next->child[ind]) { /* daca fiul nu exista, il creez */
            next->child[ind] = InitNode();
            if (!next->child[ind])
                return NULL;
        }

        /* ma deplasez la urmatorul nod */
        next = next->child[ind];
    }

    /* cand am ajuns la sfarsit, setez campul end pe TRUE */
    next->end = true;

    /* returnez ultimul nod pentru a pune pe campul info informatie */
    return next;
}

TNode Search(TNode root, char *string)
{
    TNode next = root;
    size_t i;

    /* daca key-ul calculat duce la un nod care nu exista atunci in trie
    nu exista ce caut */

    for (i = 0; i < strlen(string); i++) {
        int ind = KeyFun(string[i]);
        if (next->child[ind] == NULL)
            return NULL;

        next = next->child[ind];
    }

    return next;
}

void PrintTrieEnd(TNode root, FILE *f, TFAfi afi_fun)
{
    if (!root)
        return;

    int i;

    if (root->end == true)
        afi_fun(root->info, f);

    for (i = 0; i < NR_CH; i++)
        if (root->child[i] != NULL)
            PrintTrieEnd(root->child[i], f, afi_fun);
}

bool IsLeaf(TNode node)
{
    int i;
    for (i = 0; i < NR_CH; i++)
        if (node->child[i] != NULL)
            return false;

    return true;
}

void Delete(TNode *root, char *string, size_t c)
{
    if (!(*root))
        return;

    if (c == strlen(string)) {
        if ((*root)->end)
            (*root)->end = false;

        if (IsLeaf(*root)) {
            free((*root)->child);
            free(*root);
            *root = NULL;
        }

        return;
    }

    int ind = KeyFun(string[c]);
    Delete(&((*root)->child[ind]), string, c + 1);

    if (IsLeaf(*root) && (*root)->end == false) {
        free((*root)->child);
        free(*root);
        (*root) = NULL;
        return;
    }
}

void FreeTrie(TNode *root)
{
    if (!(*root))
        return;

    int i;
    for (i = 0; i < NR_CH; i++)
        if ((*root)->child[i])
            FreeTrie(&((*root)->child[i]));

    free((*root)->child);
    free(*root);
    (*root) = NULL;
}