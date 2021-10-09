#ifndef MALEZADANIE_INPUT_H
#define MALEZADANIE_INPUT_H

#include "commands.h"

/*
 * Wczytuje polecenia ze standardowego wejścia i wykonuje je, jeśli są poprawne.
 * Dostępne polecenia:
 * ADD - dodaje byty do drzewa [root],
 * DEL - usuwa byty z drzewa [root],
 * CHECK - sprawdza, czy dane byty występują w drzewie [root],
 * PRINT - wypisuje wszystkie byty znajdujące się bezpośrednio w podanej
 * formacji
 * W przypadku wczytania nieprawidłowego polecenia, wypisuje „ERROR”
 * na standardowe wyjście diagnostyczne.
 */
void read(Avl *root);

#endif //MALEZADANIE_INPUT_H