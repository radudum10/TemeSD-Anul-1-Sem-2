/* Dumitru Radu-Andrei - 312CB */

#include "functii.h"

int main(int argc, char **argv)
{
    FILE *fin, *fout;

    fin = fopen(argv[1], "r");
    if (!fin) {
        printf("Eroare deschidere fisier de intrare!\n");
        return -1;
    }

    fout = fopen(argv[2], "w");
    if (!fout) {
        fclose(fin);
        printf("Eroare la deschidere fisier de iesire!\n");
        return -1;
    }

    TLista *listarr;
    AQ laterque;
    Ast watchst, history;
    if (Initializare(&listarr, &laterque, &watchst, &history) == 0) {
        fclose(fin);
        fclose(fout);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    int top_len = 0; /* pt. a retine numarul el. din top10 */

    while (getline(&line, &len, fin) != -1) {

        char *operation = strtok(line, " ");
        if (operation[strlen(operation) - 1] == '\n')
            operation[strlen(operation) - 1] = '\0';

        if (strcmp(operation, "add") == 0) {
            char* date_serial = strtok(NULL, "");
            if (AddSeries(listarr, date_serial, fout) == 0) {
                free(line);
                Elibereaza(listarr, laterque, watchst, history);
                fclose(fin);
                fclose(fout);
                return -1;
            }
        }

        else if (strcmp(operation, "add_sez") == 0) {
            char *date_serial = strtok(NULL, "");
            if (AddSeason(listarr, laterque, watchst, date_serial,
                fout) == 0) {
                free(line);
                Elibereaza(listarr, laterque, watchst, history);
                fclose(fin);
                fclose(fout);
                return -1;
            }
        }

        else if (strcmp(operation, "add_top") == 0) {
                char *date_serial = strtok(NULL, "");

                if (AddTopSeries(listarr, date_serial, &top_len, fout) == 0) {
                    free(line);
                    Elibereaza(listarr, laterque, watchst, history);
                    fclose(fin);
                    fclose(fout);
                    return -1;
                }
        }

        else if (strcmp(operation, "later") == 0) {
            char *nume_serial = strtok(NULL, " ");
            if (nume_serial[strlen(nume_serial) - 1] == '\n')
                nume_serial[strlen(nume_serial) - 1] = '\0';

            if (WatchLater(laterque, listarr, nume_serial, fout,
                &top_len) == 0)
            {
                free(line);
                Elibereaza(listarr, laterque, watchst, history);
                fclose(fin);
                fclose(fout);
                return -1;
            }
        }

        else if (strcmp(operation, "watch") == 0) {
            char *nume_serial = strtok(NULL, " ");
            int durata = atoi(strtok(NULL, " "));
            if (CautaWatching(watchst, history, nume_serial, durata,
                fout) == 0)
                VizionareSerial(listarr, laterque, nume_serial, durata,
                                 watchst, history, fout, &top_len);
        }

        else if (strcmp(operation, "show") == 0) {
            char *show_type = strtok(NULL, " ");
            if (show_type[strlen(show_type) - 1] == '\n')
                show_type[strlen(show_type) - 1] = '\0';

            Afiseaza(show_type, listarr, laterque, watchst, history, fout);

        }
    }

    Elibereaza(listarr, laterque, watchst, history);
    free(line);
    fclose(fin);
    fclose(fout);

    return 0;
}