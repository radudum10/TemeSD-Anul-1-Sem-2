/* Dumitru Radu-Andrei 312CB */

iau 2 file-pointeri, unul pentru citire, unul pentru scriere
numele fisierului de citire este al 3-lea argument
numele fisierului de scriere este al 4-lea argument
citesc linie cu linie din fisier, identific tipul operatiei

initializez tabela hash, cu functia InitHash, astfel: 
- aloc memorie pentru struct-ul coresp. tabelei
- pentru vectorul din interiorul acestuia
- setez nr. maxim de "bucket-uri", pe care il primesc ca al 2 lea arg.
- setez functia de hashing(codul este dat de insumarea caracterelor
impartit la numarul maxim de bucketuri)

-> daca tipul operatiei este "put", adaug in tabela hash
(daca key-ul respectiv nu exista deja), astfel :
- pun in struct numele si ip-ul, calculez codul, adaug in lista elementul
(daca el nu exista)
- aloc o celula, ii setez campul data
- daca lista e vida, atunci inceputul listei va fi aceasta celula, iar
pointerii care indica spre urmatoarea, respectiv spre precedenta celula,
vor arata spre aceasta.
- inserez in ordine lexicografica, dupa key
- verific daca elementul care trebuie adaugat nu este chiar inceputul listei
- iau cazul particular in care trebuie adaugat la inceputul listei
- parcurg lista pentru a gasi elementul inaintea caruia trebuie sa adaug
- daca il gasesc, fac adaugarea, daca nu, adaug la sfarsitul listei

-> daca tipul operatiei este "find", caut in tabela hash, astfel:
- calculez codul, verific daca se afla la inceput, altfel parcurg lista
- returnez adresa celulei, daca gasesc
- daca returnez NULL, intorc mai apoi 0, altfel 1

-> daca tipul operatiei este "get", printez value-ul, astfel:
- cu functia precedenta gasesc celula
- printez campul "value" din data
- printez "NULL" daca nu exista key-ul dat.

-> daca tipul operatiei este "remove", elimin celula, astfel:
- cu functia de la find, gasesc celula
- daca lista in care se afla celula are un singur element si acela trebuie
eliminat, atunci eliberez continutul si celula si setez pointer-ul la NULL
- altfel, mut pointerii, verific daca trebuie sa elimin inceputul si eliberez

-> daca tipul operatiei este "print_bucket", printez lista, banal

-> daca tipul operatiei este "print", iau fiecare index, vad daca lista este
goala, altfel o printez
