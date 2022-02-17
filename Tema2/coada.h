/* Dumitru Radu-Andrei - 312CB */
#include "liste.h"

#define LEN_NUME 33
#ifndef COADA_H
#define COADA_H

typedef struct coada {
    size_t dimel;
    TLista ic, sc;
} TCoada, *AQ;

/* functii coada */

int InitQ(AQ *que, int dime); /* initalizeaza o coada */
int InsQ(AQ que, void *ae); /* insereaza un element in coada */
void *ExtrageQ(AQ que); /* extrage un element din coada */
void AfiseazaQ(AQ que, TFAfi afi_fun, FILE* fout); /* afiseaza o coada */
void DistrQ(AQ *que, TFFree free_fun); /* elibereaza o coada */
void ConcatQ(AQ surs, AQ dest); /* concateneaza 2 cozi */
#endif