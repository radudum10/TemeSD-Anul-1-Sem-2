/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

int comp_strings(void *el1, void *el2) /* compara 2 structuri de tip QueryDNS dupa campul "name" */
{
    QueryDNS* p1 = (QueryDNS *)el1;
    QueryDNS* p2 = (QueryDNS *)el2;

    return (strcmp(p1->name, p2->name));
}

void free_content(void *el) /* elibereaza o structura de tip QueryDNS */
{
    QueryDNS *p1 = (QueryDNS *)el;
    free(p1->name);
    free(p1->ip);
    free(p1);
}

TH* InitHash(size_t buckets, TFHash fh)
{
    TH *h = (TH *)calloc(1, sizeof(TH)); /* alocare tabela */
    if (!h) {
        printf("Eroare la alocare tabela hash!\n");
        return NULL;
    }

    h->arr = (TLG *)calloc(buckets, sizeof(TLG)); /* alocare vector de pointeri */
    if (!h->arr) {
        printf("Eroare la alocare vector de pointeri!\n");
        free(h);
        return NULL;
    }

    h->M = buckets; /* numarul maxim de liste din vector */
    h->fun = fh; /* functia de hashing */

    return h; 
}

void AddInHTable(TH* h, char *key, char *value)
{
    QueryDNS *el; /* pun intr-un struct cheia si valoarea corespunzatoare */
    el = (QueryDNS *)malloc(sizeof(QueryDNS));
    if (!el) {
        printf("Eroare la alocare memorie pentru struct!\n");
        return;
    }

    el->name = (char *)malloc(STR_LEN * sizeof(char));
    if (!el->name) {
        free(el);
        printf("Eroare la alocare camp nume!\n");
        return;
    }

    el->ip = (char *)malloc(STR_LEN * sizeof(char));
    if (!el->ip) {
        free(el->name);
        free(el);
        printf("Eroare la alocare camp ip!\n");
        return;
    }

    strcpy(el->name, key);
    strcpy(el->ip, value);

    int code;
    code = h->fun(key, h->M); /* calculez codul */

    int res = AddInList(h->arr+code, (void *)el, comp_strings); /* adaug elementul in lista corespunzatoare */
    if (res == 0)
        free_content((void *)el);
}

int FindElement(TH* h, char *key)
{
    int code;
    code = h->fun(key, h->M);

    if (FindInList(h->arr[code], key) == NULL)
        return 0;

    return 1;
}

void *GetValue(TH *h, char *key)
{
    int code;
    code = h->fun(key, h->M);

    TLG foundcell = FindInList(h->arr[code], key);
    if (!foundcell)
        return NULL;

    QueryDNS* foundelem = (QueryDNS *)foundcell->data;
    return (void *)foundelem->ip;
}

void RemoveFromHTable(TH *h, char *key)
{
    int code;
    code = h->fun(key, h->M);

    TLG foundcell = FindInList(h->arr[code], key);
    if (!foundcell)
        return;

    RemoveFromList(h->arr+code, foundcell, free_content);
}

void PrintHTable(TH *h, TFPrint printfun, FILE *fout)
{
    int i;
    for (i = 0; i < h->M; i++) {
        if (h->arr[i] == NULL)
            continue;
        fprintf(fout, "%d: ", i);
        PrintBucket(h->arr[i], printfun, fout);
    }
}

void FreeHTable(TH **h)
{
    int i;
/* se ia fiecare index al vectorului si eliberez lista coresp., daca este cazul */
    for (i = 0; i < (*h)->M; i++)
        if ((*h)->arr[i] != NULL)
            FreeList((*h)->arr+i, free_content);

    free((*h)->arr);
    free(*h);
    *h = NULL;
}