/* Dumitru Radu-Andrei - 312CB */
#include "coada.h"
#include "functii.h"

int InitQ(AQ *que, int dime)
{
    *que = malloc(sizeof(TCoada));
    if (!*que) {
        printf("Eroare alocare coada!\n");
        return 0;
    }

    (*que)->ic = NULL;
    (*que)->sc = NULL;
    (*que)->dimel = dime;

    return 1;
}

int InsQ(AQ que, void *ae)
{
    TLista aux;
    aux = malloc(sizeof(CelulaL));
    if (!aux) {
        printf("Eroare alocare celula!\n");
        return 0;
    }
    aux->info = ae;
    aux->urm = NULL;

    /*inserare in coada vida */
    if (que->ic == NULL) {
        que->ic = aux;
        que->sc = aux;
        return 1;
    }

    /* inserare in coada nevida*/
    que->sc->urm = aux;
    que->sc = aux;

    return 1;
}

void *ExtrageQ(AQ que)
{
    if (que->ic == NULL)
        return NULL;

    void *ae = NULL;
    TLista p = que->ic;
    que->ic = p->urm;
    ae = p->info;
    free(p);

    if (que->ic == NULL)
        que->sc = NULL;

    return ae;
}

void AfiseazaQ(AQ que, TFAfi afi_fun, FILE *fout)
{
    fprintf(fout, "[");
    TLista p;
    for (p = que->ic; p != NULL; p = p->urm) {
        afi_fun(p->info, fout);
        if (p->urm != NULL)
            fprintf(fout, ", ");
    }
    fprintf(fout, "].\n");
}

void ConcatQ(AQ surs, AQ dest)
{

    if (surs->ic == NULL)
        return;

    if (dest->ic == NULL) {
        dest->ic = surs->ic;
        dest->sc = surs->sc;
        surs->ic = surs->sc = NULL;
        return;
    }

    dest->sc->urm = surs->ic;
    dest->sc = surs->sc;

    surs->ic = surs->sc = NULL;
}

void DistrQ(AQ *que, TFFree free_fun)
{
    TLista p, u;
    for (p = (*que)->ic; p != NULL;) {
        u = p->urm;
        free_fun(p->info);
        free(p);
        p = u;
    }

    free(*que);
}