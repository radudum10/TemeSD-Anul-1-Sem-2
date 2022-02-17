/* Dumitru Radu-Andrei 312CB */

#include "utils.h"

int Hashing_Fun(void *key, size_t M) /* functie care genereaza codul */
{
    char *str = (char *)key;
    int s = 0;
    for (int i = 0; i < strlen(str); i++)
        s += (int)str[i];

    return s % M;
}

void Print_Content(void *el, FILE *fout) /* functie pentru printare */
{
    QueryDNS* x= (QueryDNS *)el;
    fprintf(fout, "%s ", x->ip);
}

int main (int argc, char **argv)
{

    FILE *fin, *fout;

    fin = fopen(argv[2], "r");
    if (!fin) {
        printf("Eroare la deschidere fisier input!\n");
        return 1;
    }

    fout = fopen(argv[3], "w");
    if (!fin) {
        printf("Eroare la creare fisier output!\n");
        fclose(fin);
        return 1;
    }

    TH* h = InitHash(atoi(argv[1]), Hashing_Fun);
    if (!h) {
        fclose(fin);
        fclose(fout);
        printf("Nu s-a reusit initializarea tabelei hash!\n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fin) != -1) {
        /* identificare tip operatie */
        char *operation = strtok(line, " ");
        if (operation[strlen(operation) - 1] == '\n')
            operation[strlen(operation) - 1] = '\0';

        if (strcmp(operation, "put") == 0) {
            char *key = strtok(NULL, " ");
            char *value = strtok(NULL, " ");
            if (value[strlen(value) - 1] == '\n') /* getline citeste cu tot cu \n */
                value[strlen(value) - 1] = '\0';

            AddInHTable(h, key, value);
        }

        else if (strcmp(operation, "find") == 0) {
            char *key = strtok(NULL, " ");
            if (key[strlen(key) - 1] == '\n')
                key[strlen(key) - 1] = '\0';

            if (FindElement(h, key) == 1)
                fprintf(fout, "True\n");
            else
                fprintf(fout, "False\n");
        }

        else if (strcmp(operation, "get") == 0) {
            char *key = strtok(NULL, " ");
            if (key[strlen(key) - 1] == '\n')
                key[strlen(key) - 1] = '\0';
            char *result = (char *)GetValue(h, key);
            if (!result)
                fprintf(fout, "NULL\n");
            else
                fprintf(fout, "%s\n", result);
        }

        else if (strcmp(operation, "remove") == 0) {
            char *key = strtok(NULL, " ");
            if (key[strlen(key) - 1] == '\n')
                key[strlen(key) - 1] = '\0';

            RemoveFromHTable(h, key);
        }

        else if (strcmp(operation, "print_bucket") == 0) {
            char *index = strtok(NULL, " ");
            if (atoi(index) < h->M)
                PrintBucket(h->arr[atoi(index)], Print_Content, fout);
        }

        else if (strcmp(operation, "print") == 0)
            PrintHTable(h, Print_Content, fout);
    }

    free(line); /* eliberare char *line folosit pt. getline */
    FreeHTable(&h);

    fclose(fin);
    fclose(fout);

    return 0;
}