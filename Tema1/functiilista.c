/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

int AddInList(TLG* list, void* el, TFCmp cmpfun)
{
    TLG aux = (TLG)malloc(sizeof(TCelulaG));
    if (!aux) {
        printf("Eroare la alocare celula!\n");
        return 0;
    }
    aux->data = el;

    /* inserare pentru lista vida */
    if (*list == NULL) {
        aux->urm = aux;
        aux->pre = aux;
        *list = aux;
        return 1;
    }

    if (cmpfun(el, (*list)->data) == 0) {
        free(aux);
        return 0;
    }

    /* inserare la inceputul listei */

    if (cmpfun(el, (*list)->data) < 0) {
        aux->pre = (*list)->pre;
        (*list)->pre->urm = aux;
        aux->urm = *list;
        (*list)->pre = aux;
        *list = aux;
        return 1;
    }

    /* inserare inainte de un elem. din interiorul listei */
    TLG p;
    for (p = (*list)->urm; p != (*list); p = p->urm) {
        if (cmpfun(el, p->data) == 0) {
            free(aux);
            return 0;
        }
        if (cmpfun(el, p->data) < 0) {
            aux->urm = p;
            aux->pre = p->pre;
            p->pre->urm = aux;
            p->pre = aux;
            return 1;
        }
    }

    /* inserare la sfarsitul listei */
    aux->urm = *list;
    aux->pre = (*list)->pre;
    (*list)->pre->urm = aux;
    (*list)->pre = aux;
    return 1;
}

TLG FindInList(TLG list, char *key)
{
    if (list == NULL) /* daca lista e vida, sigur nu gasim elementul aici */
        return NULL;

    TLG p = list;
    if (strcmp(((QueryDNS *)p->data)->name, key) == 0) /* daca se afla la inceputul listei */
        return p;

    for (p = p->urm; p != list; p = p->urm) /* daca se afla oriunde altundeva in lista */
        if (strcmp(((QueryDNS *)p->data)->name, key) == 0)
            return p;
    
    return NULL;
}

void RemoveFromList(TLG *list, TLG cell, TFFree freefun)
{
    if (*list == cell && (*list)->urm == *list) {
        freefun((*list)->data);
        free(*list);
        *list = NULL;
        return;
    }
    cell->pre->urm = cell->urm;
    cell->urm->pre = cell->pre;

    /* daca elementul este inceputul listei */
    if (*list == cell)
        *list = cell->urm;

    freefun(cell->data);
    free(cell);
    cell = NULL;
}

void PrintBucket(TLG list, TFPrint printel_fun, FILE *fout)
{
    if (!list) {
        fprintf(fout, "VIDA\n");
        return;
    }

    printel_fun(list->data, fout);
    TLG p;
    for (p = list->urm; p != list; p = p->urm)
        printel_fun(p->data, fout);

    fprintf(fout, "\n");
}

void FreeList(TLG *list, TFFree freefun)
{
    if ((*list)->urm == *list) {
        freefun((*list)->data);
        free(*list);
        *list = NULL;
        return;
    }

    TLG p, u;
    for (u = (*list)->urm; u != *list;) {
        p = u;
        u = u->urm;
        freefun(p->data);
        free(p);
    }

    freefun((*list)->data);
    free(*list);
    *list = NULL;
}