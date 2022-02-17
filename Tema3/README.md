* Dumitru Radu-Andrei 312CB

# Trie
* structura cu 3 campuri: un vector de copii, un bool care verifica daca este
* sfarsitul cuvantului si un camp de info unde se pun datele de la sfarsitul
* unei cai

## triefuns.c
* [InitNode]: aloca memorie pentru o structura coresp. unui nod din Trie si
* vectorului din interiorul acesteia.
* [KeyFun]: calculeaza index-ul din vectorul de copii al unui
* caracter conform ordinei lexicografice date
* [Insert]: insereaza intr-un trie astfel: calculeaza index-ul
* coresp. fiecarui caracter si parcurge vectorul de copii al fiecarui nod.
* Daca nu exista fiul se initializeaza, iar la sfarsit se seteaza campul de end
* pe true, se intoarce ultimul nod la care s-a ajuns.
* [Search]: cauta un sir de caractere in trie, calculeaza index-ul
* si parcurge vectorul, daca index-ul duce la un fiu care nu exista se intoarce
* NULL, altfel se intoarce nodul terminal.
* [PrintTrieEnd]: parcurge un trie, la sfarsitul unei cai afiseaza informatia
* stocata acolo(sau o parte din aceasta).
* [IsLeaf]: parcurge vectorul de copii, daca nodul are cel putin un fiu se
* returneaza fals, altfel adevarat.
* [Delete]: primeste un nod, un sir de caractere si pozitia curenta din sir,
* verifica daca nodul exista, daca s-a ajuns la finalul sirului verifica daca
* este o frunza(pentru a nu influenta alte siruri de caractere stocate),
* daca este o frunza, se elibereaza nodul, iar prin recursivitate se propaga
* mai departe.
* [FreeTrie]: primeste adresa unui nod, verifica daca nodul exista(safeguard),
* recursiv se deplaseaza pana la sfarsitul trie-ului si incepe sa elibereze.

### books.c
*[InitBook]: aloca memorie pentru o structura de tip TBook si pune informatiile
unei cart.
*[FreeBook]: elibereaza cei 2 char pointeri alocati dinamic(titlu si autor) si
apoi structura
*[AddToTries]: aloca o carte, insereaza in trie-ul cu titlurile cartilor, se
verifica inserarea(alocarea unui nod nou poate esua), daca la finalul caii se
afla deja informatie, inseamna ca acea carte exista deja se elibereaza memoria
alocata pentru carte si se iese din functie, altfel se insereaza numele
autorului in trie-ul pentru autori. Se intoarce nodul final la care s-a ajuns
la inserare, daca acesta are deja un trie pe campul de info se insereaza in el,
altfel se aloca un nod nou si se insereaza in acesta.
*[FindBook]: apeleaza [Search], daca aceasta functie a returnat NULL sau daca
nodul la care s-a ajuns nu este un nod terminal(este doar parte dintr-o cale),
inseamna ca acea carte cautata nu exista, altfel se printeaza datele cartii.
*[CompleteTitle]: prin recursivitate gaseste primele 3 noduri terminale pornind
de la un nod dat.
*[AutoCompleteBook]: cauta cu [Search] prefixul dat, apoi apeleaza CompleteTitle
pentru a gasi primele 3 carti cu acel prefix.
*[FindBookByAuthor]: cauta un autor in trie-ul cu autori, daca functia Search
returneaza NULL sau un nod care nu este terminal, atunci autorul nu exista in
trie, altfel cauta cartea fie dupa prefix fie dupa titlu.
*[DeleteBook]: cauta o carte dupa titlu, daca s-a gasit atunci se sterge din
trie-ul cu carti, apoi din trie-ul cu carti aflat in nodul terminal al numelui
autorului. Daca trie-ul autorului nu mai contine carti dupa stergere, atunci se
sterge si numele autorului din trie-ul cu autori.
*[FreeTrieCarti]: prin recursivitate se ajunge la frunze, daca nodul este
terminal atunci se elibereaza informatia din acesta(de tip TCarte), se
elibereaza vectorul de copii si nodul.

#### author.c
*[PrintTitle]: afiseaza titlul unei carti
*[FindAuthor]: functioneaza exact ca [FindBook], doar ca se cauta un autor in
trie-ul cu autori.
*[FindName]: in trie-ul cu carti al autorului, prin recursivitate gaseste un
nod terminal, din campul info se extrage numele autorului.
*[CompleteName]: se parcurge trie-ul, cand se gaseste un nod terminal se
apeleaza [FindName] pe trie-ul din info-ul nodului gasit.
*[AutoCompleteAuthor]: functioneaza ca [AutoCompleteBook]
*[FreeAuthorTrie]: asemeneea [FreeTrieCarti], dar informatia din nodurile
terminale se elibereaza cu [DeleteTrie](nu cu [FreeTrieCarti] pentru ca s-ar
elibera o structura de tip TCarte de doua ori).

##### trie.c
*[OpenFile]: deschide un fisier, intoarce pointer-ul daca s-a reusit, altfel
NULL.
*[InitTries]: aloca radacinile pentru trie-ul cu carti si pentru cel cu autor.
*[TaskCaller]: identifica tipul de operatie si apeleaza functiile.
