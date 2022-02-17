/* Dumitru Radu-Andrei - 312CB */

#include "stiva.h"

int InitS(Ast *s, int dime)
{
    *s = malloc(sizeof(TStiva));
    if (!*s) {
        printf("Eroare alocare stiva!\n");
        return 0;
    }
    (*s)->dime = dime;
    (*s)->vf = NULL;
    return 1;
}

int Push(Ast s, void *ae)
{
    TLista aux;
    aux = malloc(sizeof(CelulaL));
    if (!aux) {
        printf("Eroare alocare celula stiva!\n");
        return 0;
    }

    aux->info = ae;
    aux->urm = s->vf;
    s->vf = aux;
    return 1;
}

void *Pop(Ast s)
{
    TLista aux = s->vf;
    if (aux == NULL)
        return NULL;

    void *ae = aux->info;
    s->vf = aux->urm;
    free(aux);

    return ae;
}

void Rastoarna(Ast dest, Ast sursa)
{
    if (sursa->vf == NULL)
        return;
    TLista p = sursa->vf, u;
    while (p) {
        u = p->urm;
        p->urm = dest->vf;
        dest->vf = p;
        p = u;
    }

    sursa->vf = NULL;
}

void AfiseazaS(Ast s, TFAfi afi_fun, FILE* fout)
{
    TLista p;
    fprintf(fout, "[");
    for (p = s->vf; p != NULL; p = p->urm) {
        afi_fun(p->info, fout);
        if (p->urm != NULL)
            fprintf(fout, ", ");
    }

    fprintf(fout, "].\n");
}

void DistrS(Ast *s, TFFree free_fun)
{
    TLista p, u;
    for (p = (*s)->vf; p != NULL; p = u) {
        u = p->urm;
        free_fun(p->info);
        free(p);
    }

    free(*s);
    *s = NULL;
}