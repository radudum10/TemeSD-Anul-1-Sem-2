/* Dumitru Radu-Andrei - 312CB */

#include "liste.h"
#include "coada.h"
#include "stiva.h"

#ifndef FUNCTII_H
#define FUNCTII_H

typedef struct sezon_info {
    size_t nr_ep;
    AQ episods;
} Sezon;

typedef struct serial_info {
    int id;
    char *nume;
    float rating;
    size_t nr_sez;
    AQ sezoane;
} Serial;

typedef struct seriale_top {
    size_t poz;
    Serial *ser;
} SerialTop;


typedef int (*TFCmpN)(char *, void *); /* compara un sir de caractere cu numele serialului */

/* functii implementate */

void AfiseazaSerialTop(void *el, FILE *fout);
int Initializare(TLista **listarr, AQ *laterque, Ast *watchst, Ast *history); /* initializeaza structurile care
                                                                              vor fi folosite pt. memorarea serialelor */
int SeasonBuilder(char **date_sezon, Sezon **sez); /* construieste un sezon */
int SeriesBuilder(char *date_serial, Serial **ser); /* construieste un serial */
int AddSeries(TLista *listarr, char *date_serial, FILE *fout); /* adauga un serial in lista */
void Afiseaza(char *show_type, TLista *listarr, AQ laterque, Ast watchst,
              Ast history, FILE *fout); /* afiseaza o categorie / coada later / stiva watch / stiva history */
int AddSeason(TLista *listarr, AQ laterq, Ast watchst, char *date_serial,
              FILE *fout); /* adauga un sezon la un serial */
int AddTopSeries(TLista *listarr, char *date_serial, int *index_top, FILE* fout); /* adauga un serial la top10 */
int WatchLater(AQ que, TLista *listarr, char *nume_serial,
               FILE *fout, int *top_len); /* muta un serial din categoria lui in watch later */
void VizionareSerial(TLista *listarr, AQ laterq, char *nume_serial, int durata,
                    Ast watchst, Ast history, FILE *fout, int *top_len); /* cauta un serial care nu e deja in stiva de watching
                                  si calculeaza timpul ramas, muta in watching / history */
int CautaWatching(Ast watchingst, Ast history, char *nume_serial, int durata,
                  FILE *fout); /* cauta un serial in stiva currently watching
                                calculeaza timpul ramas, eventual muta*/
void Elibereaza(TLista *listarr, AQ laterq, Ast watch, Ast history); /* elibereaza memoria alocata */
#endif