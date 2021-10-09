#ifndef MALEZADANIE_AVL_H
#define MALEZADANIE_AVL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Typy reprezentujące węzeł drzewa AVL zmiennych typu (char *) oraz drzewo AVL
 * zmiennych typu (char *). Każdy taki węzeł zawiera drzewo AVL (wskaźnik
 * na korzeń innego drzewa).
 */
struct Node;

typedef struct Node* Avl;

struct Node {
    char *value;
    int height;
    Avl left, right;
    Avl inner_tree;
};

/*
 * Sprawdza, czy w drzewie [tree] występuje wartość [value]. Jeśli tak, zwraca
 * wskaźnik na ten wierzchołek (drzewo o korzeniu zawierającym wartość [value]).
 * W przeciwnym przypadku zwraca NULL.
 */
Avl exist(Avl tree, char *value);

/*
 * Wypisuje wszystkie wartości występujące w drzewie [tree] w porządku
 * leksykograficznym.
 */
void print_lexicographically(Avl tree);

/*
 * Wstawia do drzewa wskazywanego przez wskaźnik [tree] wartość [value],
 * o ile nie występuje ona dotychczas w tym drzewie.
 */
void insert(Avl *tree, char *value, Avl *root);

/*
 * Usuwa wszystkie wierzchołki drzewa wskazywanego przez wskaźnik [tree].
 */
void delete_whole_tree(Avl *tree);

/*
 * Usuwa wartość [value] z drzewa, wskazywanego przez wskaźnik [tree] (o ile
 * taka wartość występuje w tym drzewie).
 */
void delete(Avl *tree, char *value);

#endif //MALEZADANIE_AVL_H