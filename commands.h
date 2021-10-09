#ifndef MALEZADANIE_COMMANDS_H
#define MALEZADANIE_COMMANDS_H

#include "avl.h"

/*
 * Typ reprezentujący formacje (argumenty) podawane w komendach.
 */
struct nature {
    char *forest;
    char *tree;
    char *animal;
};

/*
 * Dodaje formację [new_creation] do drzewa, wskazywanego przez wskaźnik [root].
 * Dodaje tylko te byty, których brakuje. Poprawne dodanie bytów do drzewa jest
 * sygnalizowane komunikatem „OK” wypisywanym na standardowe wyjście.
 */
void add(struct nature new_creation, Avl *root);

/*
 * Wypisuje wszystkie byty w danej formacji na standardowe wyjście.
 */
void print(struct nature creation, Avl root);

/*
 * Usuwa dany byt z drzewa wskazywanego przez wskaźnik [root]. Jeśli funkcja
 * zostanie wywołana bez żadnych argumentów, usuwa całe drzewo. Poprawne
 * usunięcie bytów z drzewa jest sygnalizowane komunikatem „OK” wypisywanym
 * na standardowe wyjście.
 */
void del(struct nature creation, Avl *root);

/*
 * Sprawdza, czy formacja [creation] istnieje w drzewie, wskazywanym
 * przez wskaźnik [root]. Wypisuje na standardowe wyjście "YES" lub "NO". Jako
 * wartość argumentu, z wyjątkiem ostatniego, można podać gwiazdkę.
 * Gwiazdka ‘*’ jest metaznakiem, pasującym do dowolnej nazwy.
 */
void check(struct nature creation, Avl root);

#endif //MALEZADANIE_COMMANDS_H