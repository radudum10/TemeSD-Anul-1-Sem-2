/* Dumitru Radu-Andrei 312CB */

#ifndef TRIE_H
#define TRIE_H

typedef struct NodeTrie {
    struct NodeTrie **child; /* vectorul in care se retin copiii nodului */
    bool end; /* end este adevarat daca s-a ajuns la sfarsitul unui cuvant */
    void *info; /* un pointer catre o structura cu informatiile cartii */
} Node, *TNode;

typedef void (*TFAfi)(void *, FILE *);

TNode InitNode(); /* aloca si initalizeaza un nod */
TNode Insert(TNode root, char *string); /* insereaza un string intr-un trie */
int AddToTries (TNode fir, TNode sec, char *date_carte); /* adauga o carte in primul trie + autorul in al doilea */
TNode Search(TNode root, char *string); /* cauta un string intr-un trie */
void PrintTrieEnd(TNode root, FILE *f, TFAfi afi_fun); /* afiseaza informatia de la sf. unei cai */
void Delete(TNode *root, char *string, size_t c); /* sterge un string din trie */
bool IsLeaf(TNode node); /* true daca nodul nu are fii, false altfel */
void FreeTrie(TNode *root); /* elibereaza memoria alocata pentru un trie fara informatii aditionale */

#endif