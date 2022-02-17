/* Dumitru Radu-Andrei - 312CB */
#include "liste.h"
#include "functii.h"

/* daca se adauga in top un el. cu nr. de ordine egal cu unul existent,
 incrementez numerele de ordine */
void IncrementeazaTop(TLista L)
{
    for (; L != NULL; L = L->urm) {
        SerialTop *sertop = (SerialTop *)(L->info);
        sertop->poz++;
    }
}

int InserareOrd(TLista *L, void *ae, TFCmp cmp)
{
    TLista aux = malloc(sizeof(CelulaL));
    if (!aux) {
        printf("Eroare alocare celula lista!\n");
        return 0;
    }

    aux->info = ae;
    aux->urm = NULL;

    int i = 1;

    if (*L == NULL) {
        *L = aux;
        return i;
    }
    for (; *L != NULL ; L = &((*L)->urm)) {
        if (cmp(ae, (*L)->info) >= 0) {
            if (cmp(ae, (*L)->info) == 0) { /* poate fi 0 doar pt. serialele din top 10 */
                IncrementeazaTop(*L);
            }
            aux->urm = *L;
            (*L) = aux;
            return i;
        }
        i++;
    }

    (*L) = aux;

    return i;
}

void AfiseazaLista(TLista L, TFAfi AfiSerial, FILE *fout)
{
    fprintf(fout, "[");
    for (; L != NULL; L = L->urm) {
        AfiSerial(L->info, fout);

        if (L->urm != NULL)
            fprintf(fout, ", ");
    }
    fprintf(fout, "].\n");
}

void DistrugeLista(TLista *L, TFFree free_fun)
{
    if (*L == NULL)
        return;
    TLista p, u;
    for (p = *L; p != NULL;) {
        u = p->urm;
        free_fun(p->info);
        free(p);
        p = u;
    }

    *L = NULL;
}