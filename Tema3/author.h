/* Dumitru Radu-Andrei 312CB */

#ifndef AUTHOR_H
#define AUTHOR_H

void FindAuthor(TNode root, char *author, FILE *f); /* cauta un autor si afiseaza toate cartile lui */
void AutoCompleteAuthor(TNode root, char *prefix, FILE *f); /* cauta un autor in al doilea trie */
void FreeAuthorTrie(TNode *root); /* elibereaza memoria alocata pentru trie-ul unde se retin autorii */

#endif