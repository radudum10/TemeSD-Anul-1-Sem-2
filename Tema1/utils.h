/* Dumitru Radu-Andrei 312CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UTILS_H
#define UTILS_H

#define STR_LEN 50

typedef int (*TFHash)(void*, size_t);  /* functie pentru determinarea codului */
typedef void (*TFPrint)(void *, FILE*); /* functie pentru printare element */
typedef int (*TFCmp)(void *, void *); /* functie pentru comparare */
typedef void (*TFFree)(void *); /* functie pentru eliberare */

typedef struct query {
    char *name;
    char *ip;
} QueryDNS;

typedef struct celula {
    struct celula *urm; /* adresa celulei urmatoare */
    struct celula *pre; /* adresa celulei anterioare */
    void *data; /* adresa informatie */
} TCelulaG, *TLG;

typedef struct hashtable {
    size_t M; /* numarul de bucket-uri din tabela HASH */
    TFHash fun; /* functia HASH */
    TLG* arr; /* vectorul de pointeri */
} TH;

/* functii tabela hash */

TH* InitHash(size_t buckets, TFHash fh); /* initalizeaza tabela hash */
void AddInHTable(TH* h, char *key, char *value); /* adauga in tabela hash */
int FindElement(TH *h, char *key);  /* daca gaseste elementul dorit, returneaza adresa */
void *GetValue(TH *h, char *key); /* returneaza un ip care are asociat o anumita cheie */
void RemoveFromHTable(TH *h, char *key); /* sterge elementul cu o anumita cheie */
void PrintHTable(TH *h, TFPrint printel_fun, FILE *fout); /* afiseaza tabela hash */
void FreeHTable(TH **h);

/* functii lista */
int AddInList(TLG* list, void* el, TFCmp cmpfun); /* adauga in lista */
TLG FindInList(TLG list, char *key); /* cauta in lista */
void RemoveFromList(TLG *list, TLG cell, TFFree freefun); /* sterge din lista */
void PrintBucket(TLG list, TFPrint printel_fun, FILE *fout); /* afiseaza o lista */
void FreeList(TLG *list, TFFree freefun); /* distruge o lista */

#endif 

