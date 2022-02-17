/* Dumitru Radu-Andrei - 312CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NR_LISTE 4

#ifndef LISTE_H
#define LISTE_H

typedef struct cellist {
    void *info;
    struct cellist *urm;
} CelulaL, *TLista;

typedef int (*TFCmp)(void *, void *);
typedef void (*TFAfi)(void *, FILE *);
typedef void (*TFFree)(void *);

/* functii lista */
int InserareOrd(TLista *L, void *ae, TFCmp cmp); /* insereaza ordonat in lista in functie de cmp */
void AfiseazaLista(TLista L, TFAfi Serial, FILE* fout); /* afiseaza o lista */
void DistrugeLista(TLista *L, TFFree free_fun); /* elibereaza memoria alocata pt. lista */
#endif