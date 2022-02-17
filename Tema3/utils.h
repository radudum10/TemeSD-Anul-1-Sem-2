/* Dumitru Radu-Andrei 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "trie.h"
#include "books.h"
#include "author.h"

#ifndef UTILS_H
#define UTILS_H

#define SPECIAL_CH ".-'?! " /* caracterele speciale din ordinea lexicografica data */
#define NR_LOWCASE 26 /* numarul de litere mici */
#define NR_UPPCASE 26 /* numarul de litere mari */
#define NR_DIGITS 10 /* numarul de cifre */
#define NR_CH 68 /* numarul de caractere in total */

#define ASCII_START_LOWCASE 97 /* pozitia de inceput a literelor mici in tabela ASCII */
#define ASCII_START_UPCASE 65 /* pozitia de inceput a literelor mari in tabela ASCII */

#endif