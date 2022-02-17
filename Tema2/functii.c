/* Dumitru Radu-Andrei - 312CB */

#include "functii.h"

int Initializare(TLista **listarr, AQ *laterque, Ast *watchst, Ast *history)
{
    *listarr = malloc(NR_LISTE * sizeof(CelulaL));
    if (!*listarr) {
        printf("Eroare alocare vector de liste!\n");
        return 0;
    }

    int i;
    /* initalizez fiecare lista din vector cu NULL */
    for (i = 0; i < NR_LISTE; i++)
        (*listarr)[i] = NULL;

    if (InitQ(laterque, sizeof(Serial)) == 0) {
        printf("Eroare alocare coada later!\n");
        free(*listarr);
        return 0;
    }

    if (InitS(watchst, sizeof(Serial)) == 0) {
        printf("Eroare alocare stiva watching!\n");
        free(*listarr);
        free(laterque);
        return 0;
    }

    if (InitS(history, sizeof(Serial)) == 0) {
        printf("Eroare alocare stiva history!\n");
        free(*listarr);
        free(laterque);
        free(watchst);
        return 0;
    }

    return 1;
}

/* aloca memorie pentru informatia unui episod */
void *AlocaInfo(int value)
{
    int *adr;
    adr = malloc(sizeof(int));
    if (!adr) {
        printf("Eroare la alocare memorie pt. info!\n");
        return NULL;
    }
    *adr = value;
    return (void *)adr;
}

/* construieste un struct pt. sezon, 0 = construire esuata, 1 = construire reusita */
int SeasonBuilder(char **date_sezon, Sezon **sez)
{
    /* alocare sezon */
    *sez = malloc(sizeof(Sezon));
    if (!(*sez)) {
        printf("Eroare alocare struct sezon!\n");
        return 0;
    }
    (*sez)->nr_ep = atoi(strtok(*date_sezon, " "));
    if (InitQ(&((*sez)->episods), sizeof(int)) == 0) {
        free(sez);
        return 0;
    }

    int i;
    for (i = 0; i < (*sez)->nr_ep; i++) {
        /* iau fiecare episod si il bag in coada */
        int ep_len = atoi(strtok(NULL, " "));
        if (InsQ((*sez)->episods, AlocaInfo(ep_len)) == 0) {
            free(sez);
            return 0;
        }
    }

    return 1;
}

/* construieste un struct pt. serial, 0 = construire esuata, 1 = construire reusita */
int SeriesBuilder(char *date_serial, Serial **ser)
{
    (*ser)->nume = malloc(LEN_NUME * sizeof(char));
    if (!(*ser)->nume) {
        printf("Eroare alocare nume serial!\n");
        free(*ser);
        return 0;
    }
    strcpy((*ser)->nume, strtok(date_serial, " "));

    (*ser)->rating = atof(strtok(NULL, " "));
    (*ser)->nr_sez = atoi(strtok(NULL, " "));

    if (InitQ(&((*ser)->sezoane), sizeof(Sezon)) == 0) {
        free((*ser)->nume);
        free(*ser);
        return 0;
    }

    /* construiesc sezonul si il bag in coada */
    int i;
    for (i = 0; i < (*ser)->nr_sez; i++) {
        Sezon *sez = NULL;
        char *date_sezon = strtok(NULL, "");

        if (SeasonBuilder(&date_sezon, &sez) == 0) {
            free((*ser)->nume);
            free(*ser);
            return 0;
        }

        if (InsQ((*ser)->sezoane, sez) == 0) {
            free((*ser)->nume);
            free(*ser);
            DistrQ(&(sez->episods), free);
            free(sez);
            return 0;
        }
    }
    return 1;
}

/* elibereaza un serial */
void FreeSeries(void *el)
{
    Serial *ser = (Serial *)el;
    free(ser->nume);
    TLista p, u;
    for (p = ser->sezoane->ic ; p != NULL;) {
        u = p->urm;
        Sezon *sezon = (Sezon *)p->info;
        DistrQ(&(sezon->episods), free);
        free(sezon);
        free(p);
        p = u;
    }
    free(ser->sezoane);
    free(ser);
}

/* compara 2 seriale dupa rating sau dupa nume */
int ComparaSeriale(void *el1, void *el2)
{
    Serial *ser1 = (Serial *)el1;
    Serial *ser2 = (Serial *)el2;

    if (ser1->rating > ser2->rating)
        return 1;

    else if (ser1->rating < ser2->rating)
        return -1;

    return strcmp(ser2->nume, ser1->nume);
}

/* elibereaza un serial din top */
void FreeTopSeries(void *el)
{
    SerialTop *sertop = (SerialTop *)el;
    FreeSeries(sertop->ser);
    free(sertop);
}

int AddSeries(TLista *listarr, char *date_serial, FILE *fout)
{
    /* alocare serial */
    Serial *ser = malloc(sizeof(Serial));
    if (!ser) {
        printf("Eroare alocare struct serial!\n");
        return 0;
    }

    ser->id = atoi(strtok(date_serial, " "));
    date_serial = strtok(NULL, "");

    if (SeriesBuilder(date_serial, &ser) == 0) {
        free(ser);
        return 0;
    }

    int i = InserareOrd(&listarr[ser->id - 1], ser, ComparaSeriale);

    if (i == 0) {
        FreeSeries(ser);
        return 0;
    }

    fprintf(fout, "Serialul %s a fost adaugat la pozitia %d.\n", ser->nume, i);
    return 1;
}
/* daca top-ul este plin, adaug elementul pe pozitia dorita
si shiftez la dreapta */

void ModifyTop(TLista *L, void *ae, TFCmp cmp)
{
    void *ainfo;

    for (; *L != NULL ; L = &((*L)->urm)) {

        if (cmp(ae, (*L)->info) == 0) { /* caut serialul cu pozitia identica */
            SerialTop *sertop = (SerialTop *)((*L)->info);
            sertop->poz++;
            ainfo = (*L)->info;
            (*L)->info = ae;
            TLista p;

            /* incrementez numarul de ordine si mut serialul spre dreapta */
            for (p = (*L)->urm; p != NULL; p = p->urm) {
                sertop = (SerialTop *)(p->info);
                sertop->poz++;
                void *currentinfo = p->info;
                p->info = ainfo;
                ainfo = currentinfo;
            }
            break;
        }
    }
    FreeTopSeries(ainfo);
}

/* compara 2 seriale dupa pozitia din top */
int ComparaSerialeTop(void *el1, void *el2)
{
    SerialTop *ser1 = (SerialTop *)el1;
    SerialTop *ser2 = (SerialTop *)el2;

    return ser2->poz - ser1->poz;
}

int AddTopSeries(TLista *listarr, char *date_serial, int *top_len, FILE *fout)
{
    SerialTop *sertop = malloc(sizeof(SerialTop));
    if (!sertop) {
        printf("Nu a reusit alocare struct serial top!\n");
        return 0;
    }

    sertop->ser = malloc(sizeof(Serial));
    if (!sertop->ser) {
        printf("Nu a reusit alocare struct serial!\n");
        free(sertop);
        return 0;
    }
    sertop->ser->id = 4; /* serialele din top sunt in a 4 a lista */
    sertop->poz = atoi(strtok(date_serial, " "));

    date_serial = strtok(NULL, "");

    if (SeriesBuilder(date_serial, &(sertop->ser)) == 0) {
        FreeSeries(sertop->ser);
        free(sertop);
        return 0;
    }

    if ((*top_len) == 10) {
        ModifyTop(&listarr[sertop->ser->id - 1], sertop, ComparaSerialeTop);
        fprintf(fout, "Categoria top10: ");
        AfiseazaLista(listarr[3], AfiseazaSerialTop, fout);
        return 1;
    } /* daca top-ul este plin */

    else if (InserareOrd(&listarr[sertop->ser->id - 1], sertop,
    ComparaSerialeTop) == 0) {
        FreeSeries(sertop->ser);
        free(sertop);
        return 0;
    }
    fprintf(fout, "Categoria top10: ");
    AfiseazaLista(listarr[3], AfiseazaSerialTop, fout);
    (*top_len)++;
    return 1;
}

/* afiseaza un serial */
void AfiseazaSerial(void *el, FILE *fout)
{
    Serial *ser = (Serial *)el;
    fprintf(fout, "(%s, %.1f)", ser->nume, ser->rating);
}

/*afiseaza un serial din top10 */
void AfiseazaSerialTop(void *el, FILE *fout)
{
    SerialTop *sertop = (SerialTop *)el;
    fprintf(fout, "(%s, %.1f)", sertop->ser->nume, sertop->ser->rating);
}

void Afiseaza(char *show_type, TLista *listarr, AQ laterque, Ast watchst,
              Ast history, FILE *fout)
{
    if (strcmp(show_type, "top10") == 0) {
        fprintf(fout, "Categoria top10: ");
        AfiseazaLista(listarr[3], AfiseazaSerialTop, fout);
    }

    else if (strcmp(show_type, "later") == 0) {
        fprintf(fout, "Categoria later: ");
        AfiseazaQ(laterque, AfiseazaSerial, fout);
    }

    else if (strcmp(show_type, "watching") == 0) {
        fprintf(fout, "Categoria watching: ");
        AfiseazaS(watchst, AfiseazaSerial, fout);
    }

    else if (strcmp(show_type, "history") == 0) {
        fprintf(fout, "Categoria history: ");
        AfiseazaS(history, AfiseazaSerial, fout);
    }

    else {
        int index;
        index = atoi(show_type);
        fprintf(fout, "Categoria %d: ", index);
        AfiseazaLista(listarr[index - 1], AfiseazaSerial, fout);
    }
}

/* cauta un serial intr-o lista (care nu e top)*/
void GasesteSerial(char *nume_serial, TLista L, Serial **ser)
{
    for (; L != NULL; L = L->urm) {
        Serial *el = (Serial *)(L->info);
        if (strcmp(el->nume, nume_serial) == 0) {
            *ser = L->info;
            return;
        }
    }
}

/* cauta in categoria top */
void GasesteSerialTop(char *nume_serial, TLista L, Serial **ser)
{
    for (; L != NULL; L = L->urm) {
        SerialTop *el = (SerialTop *)(L->info);
        if (strcmp(el->ser->nume, nume_serial) == 0) {
            *ser = el->ser;
            return;
        }
    }
}

/* cauta un serial in coada later */
void GasesteSerialLater(char *nume_serial, AQ laterq, Serial **ser)
{
    if (laterq->ic == NULL)
        return;

    AQ aux;
    if (InitQ(&aux, sizeof(Serial)) == 0)
        return;

    void *ae = ExtrageQ(laterq);

    while (ae) {
        Serial *el = (Serial *)ae;
        if (strcmp(el->nume, nume_serial) == 0) {
        /* gasesc elementul si il pun in coada */
            *ser = el;
            InsQ(aux, ae);
            break;
        }
        InsQ(aux, ae);
        ae = ExtrageQ(laterq);
    }

    /* refac coada initiala si o eliberez pe cea auxiliara */
    ConcatQ(laterq, aux);
    ConcatQ(aux, laterq);
    free(aux);
}

/* cauta un serial in stiva de watching */
void GasesteSerialWatching(char *nume_serial, Ast watchst, Serial **ser)
{
    if (watchst->vf == NULL)
        return;

    Ast aux;
    if (InitS(&aux, sizeof(Serial)) == 0) {
        return;
    }

    aux->vf = NULL;
    aux->dime = sizeof(Serial);

    void *ae = Pop(watchst);
    while (ae) {
        Serial *el = (Serial *)ae;
        if (strcmp(el->nume, nume_serial) == 0) {
            *ser = el;
            Push(watchst, ae);
            break;
        }
        Push(aux, ae);
        ae = Pop(watchst);
    }

    Rastoarna(watchst, aux);
    free(aux);
    return;
}

/* calculeaza durata unui sezon */
int CalculeazaDurataSez(void *el)
{
    Sezon *sez = (Sezon *)el;

    int rez = 0;
    TLista p;
    for (p = sez->episods->ic; p != NULL; p = p->urm) {
        int *ep_len = (int *)(p->info);
        rez += *ep_len;
    }
    return rez;
}

/* calculeaza durata unui serial */
int CalculeazaDurataSer(AQ sezoane)
{
    int rez = 0;
    TLista p;
    for (p = sezoane->ic; p != NULL; p = p->urm)
        rez += CalculeazaDurataSez(p->info);

    return rez;
}

int AddSeason(TLista *listarr, AQ laterq, Ast watchst, char *date_serial,
              FILE *fout)
{
    char *nume_serial = strtok(date_serial, " ");
    Sezon *sez = NULL;
    char *date_sezon = strtok(NULL, "");
    if (SeasonBuilder(&date_sezon, &sez) == 0)
        return 0;

    Serial *ser = NULL;
    GasesteSerialTop(nume_serial, listarr[3], &ser); /* caut in top */
    if (ser == NULL) { /* daca nu l-am gasit in top*/
        int i;
        for (i = 0; i < NR_LISTE - 1; i++) { /* caut in restul categoriilor */
            GasesteSerial(nume_serial, listarr[i], &ser);
            if (ser != NULL)
                break;
        }
    }

    if (ser == NULL) { /* daca nu l-am gasit in categorii */
        GasesteSerialLater(nume_serial, laterq, &ser); /* caut in later */
        if (ser == NULL) { /* daca nu l-am gasit in later */
            GasesteSerialWatching(nume_serial, watchst, &ser); /* caut in watching*/
            ser->id += CalculeazaDurataSez(sez); /* daca l-am gasit in watching, actualizez durata */
        }
    }

    if (InsQ(ser->sezoane, sez) == 0) {
        DistrQ(&(sez->episods), free);
        free(sez);
        return 0;
    }
    fprintf(fout, "Serialul %s are un sezon nou.\n", nume_serial);
    return 1;
}

void *ExtrSerial(TLista *L, char* nume_serial, TFCmpN cmp)
{
    TLista p, u = NULL;
    void *rez = NULL;
    for (p = *L; p != NULL; u = p, p = p->urm) {
        if (cmp(nume_serial, p->info) == 0) {
            rez = p->info;

            if (u == NULL) {
                *L = (*L)->urm;
                free(p);
                return rez;
            }

            u->urm = p->urm;
            free(p);
            return rez;
        }
    }

    return rez;
}

void DecrementeazaTop(TLista L)
{
    SerialTop *sertop;
    for (; L != NULL; L = L->urm) {
        sertop = (SerialTop *)(L->info);
        sertop->poz--;
    }
}

void *ExtrSerTop(TLista *L, char* nume_serial, TFCmpN cmp, int *top_len)
{
    TLista p, u = NULL;
    void *rez = NULL;

    /* daca gasesc serialul, scad lungimea topului si decrementez nr. de ordine */
    for (p = *L; p != NULL; u = p, p = p->urm) {
        if (cmp(nume_serial, p->info) == 0) {
            rez = p->info;
            (*top_len)--;
            if (u == NULL) {
                *L = (*L)->urm;
                free(p);
                if (*L != NULL)
                    DecrementeazaTop(*L);
                return rez;
            }

            u->urm = p->urm;
            free(p);
            DecrementeazaTop(u->urm);
            return rez;
        }
    }

    return rez;
}

/* compara un sir de caractere dat cu numele unui serial */
int CmpNumeSer(char *nume, void *el)
{
    Serial *ser = (Serial *)el;

    return strcmp(ser->nume, nume);
}

/* compara un sir de caractere dat cu numele unui serial din top */
int CmpNumeSerTop(char *nume, void *el)
{
    SerialTop *sertop = (SerialTop *)el;
    return strcmp(sertop->ser->nume, nume);
}

int CountQueue(AQ que)
{
    int counter = 0;
    TLista p;
    for (p = que->ic; p != NULL; p = p->urm)
        counter++;

    return counter;
}

Serial *Extrage(TLista *listarr, char *nume_serial, int *top_len)
{
    int i;
    Serial *ser = NULL;
    for (i = 0; i < NR_LISTE; i++) {
        /* caut in primele 2 liste */
        if (i < 3) {
            ser = (Serial *)ExtrSerial(&listarr[i], nume_serial, CmpNumeSer);
            if (ser) /* daca gaseste serialul, nu il mai caut */
                return ser;
        }

        /*caut in top */
        else if (i == 3) {
            SerialTop *sertop = (SerialTop *)ExtrSerTop(&listarr[i],
            nume_serial, CmpNumeSerTop, top_len);
            if (sertop) {
                ser = sertop->ser;
                free(sertop);
                return ser;
            }
        }
    }
    return ser;
}

int WatchLater(AQ que, TLista *listarr, char *nume_serial,
               FILE *fout, int *top_len)
{
    Serial *ser = Extrage(listarr, nume_serial, top_len);

    if (InsQ(que, ser) == 0)
        return 0;

    int pos = CountQueue(que);
    fprintf(fout, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n",
            ser->nume, pos);

    return 1;
}

void VizionareSerial(TLista *listarr, AQ laterq, char *nume_serial, int durata,
                    Ast watchst, Ast history, FILE *fout, int *top_len)
{
    Serial *ser = Extrage(listarr, nume_serial, top_len);
    if (ser != NULL) {
        /* folosesc campul de ID pt a memora durata unui serial */
        ser->id = CalculeazaDurataSer(ser->sezoane) - durata;
        if (ser->id <= 0) {
            fprintf(fout, "Serialul %s a fost vizionat integral.\n",
                    ser->nume);
            Push(history, ser);
        }
        else
            Push(watchst, ser);
        return;
    }

    /* caut in watchlater daca nu am gasit in liste */
    AQ auxque;
    if (InitQ(&auxque, sizeof(Serial)) == 0)
        return;

    void *ae = ExtrageQ(laterq);
    while (ae) {
        if (CmpNumeSer(nume_serial, ae) == 0) {
            ser = (Serial *)ae;
            ser->id = CalculeazaDurataSer(ser->sezoane) - durata;
            if (ser->id <= 0) {
                Push(history, ser);
                fprintf(fout, "Serialul %s a fost vizionat integral.\n",
                        ser->nume);
            }
            else
                Push(watchst, ser);
            break;
        }
        InsQ(auxque, ae);
        ae = ExtrageQ(laterq);
    }
    ConcatQ(laterq, auxque);
    ConcatQ(auxque, laterq);
    free(auxque);
}

int CautaWatching(Ast watchingst, Ast history, char *nume_serial, int durata,
                  FILE* fout)
{
    if (watchingst->vf == NULL)
        return 0;

    Ast aux;
    if (InitS(&aux, sizeof(Serial)) == 0)
        return 0;

    void *ae = Pop(watchingst);
    while (ae) {
        if (CmpNumeSer(nume_serial, ae) == 0) {
            Serial *ser = (Serial *)ae;
            ser->id -= durata;
            if (ser->id <= 0) {
                Push(history, ae);
                fprintf(fout, "Serialul %s a fost vizionat integral.\n",
                        ser->nume);
                Rastoarna(watchingst, aux);
                free(aux);
            }
            else {
                Rastoarna(watchingst, aux);
                free(aux);
                Push(watchingst, ae);
            }

            return 1;
        }
        Push(aux, ae);
        ae = Pop(watchingst);
    }

    Rastoarna(watchingst, aux);
    free(aux);
    if (ae == NULL)
        return 0;
    return 1;
}

void Elibereaza(TLista *listarr, AQ laterq, Ast watch, Ast history)
{
    int i;
    for (i = 0; i < NR_LISTE - 1; i++) /* eliberez categoriile mai putin top-ul */
        DistrugeLista(&(listarr[i]), FreeSeries);

    DistrugeLista(&listarr[3], FreeTopSeries);

    DistrQ(&laterq, FreeSeries);
    DistrS(&watch, FreeSeries);
    DistrS(&history, FreeSeries);
    free(listarr);
}