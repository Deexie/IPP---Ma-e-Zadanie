#include "avl.h"

static int max(int a, int b);

static int height(Avl node);

static int find_height(Avl tree);

static int height_difference(Avl tree);

/*
 * Rotuje drzewo wskazywane przez wskaźnik [tree] tak, że prawy syn korzenia
 * tego drzewa, staje się jego korzeniem.
 */
static void left_rotation(Avl *tree);

/*
 * Rotuje drzewo wskazywane przez wskaźnik [tree] tak, że lewy syn korzenia tego
 * drzewa, staje się jego korzeniem.
 */
static void right_rotation(Avl *tree);

/*
 * Balansuje drzewo wskazywane przez wskaźnik [tree], tak, aby wysokości lewego
 * i prawego poddrzewa tego drzewa różniły się o nie więcej niż jeden.
 */
static void balance(Avl *tree);

/*
 * Zwraca nowy wierzchołek drzewa Avl o wartości [value].
 */
static Avl create_new_node(char *value, unsigned long str_length);

/*
 * Usuwa korzeń drzewa, wskazywanego przez [tree].
 */
static void delete_node(Avl *tree);

/*
 * Zamienia wierzchołek drzewa, wskazywanego przez [tree] z najmniejszym
 * potomkiem prawego poddrzewa tego drzewa.
 */
static void swap_root_with_minimum(Avl *tree);

/*
 * Usuwa wierzchołek drzewa, wskazywanego przez [tree].
 */
static void delete_root_of_tree(Avl *tree, char *node_value);



static int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}


static int height(Avl node) {
    if (node == NULL)
        return 0;
    return node->height;
}


static int find_height(Avl tree) {
    return (max(height(tree->left), height(tree->right)) + 1);
}


static int height_difference(Avl tree) {
    return (height(tree->left) - height(tree->right));
}


static void left_rotation(Avl *tree) {
    Avl old_root = *tree;
    *tree = old_root->right;

    old_root->right = (*tree)->left;
    (*tree)->left = old_root;

    ((*tree)->left)->height = find_height((*tree)->left);
    (*tree)->height = find_height(*tree);
}


static void right_rotation(Avl *tree) {
    Avl old_root = *tree;
    *tree = old_root->left;

    old_root->left = (*tree)->right;
    (*tree)->right = old_root;

    ((*tree)->right)->height = find_height((*tree)->right);
    (*tree)->height = find_height(*tree);
}


static void balance(Avl *tree) {
    if (height_difference(*tree) > 1) {
        if (height_difference((*tree)->left) >= 0) {
            right_rotation(tree);
        }
        else {
            left_rotation(&((*tree)->left));
            right_rotation(tree);
        }
    }
    else if (height_difference(*tree) < -1) {
        if (height_difference((*tree)->right) <= 0) {
            left_rotation(tree);
        }
        else {
            right_rotation(&((*tree)->right));
            left_rotation(tree);
        }
    }
    else {
        (*tree)->height = find_height(*tree);
    }
}


static Avl create_new_node(char *value, unsigned long str_length) {
    Avl node = malloc(sizeof(struct Node));
    if (node == NULL) {
        // Nie można zaalokować pamięci.
        exit(1);
    }
    else {
        node->value = malloc((str_length + 1) * sizeof(char));
        if (node->value == NULL) {
            // Nie można zaalokować pamięci.
            exit(1);
        }

        strcpy(node->value, value);
        node->height = 1;
        node->inner_tree = NULL;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}


static void delete_node(Avl *tree) {
    if (tree != NULL && *tree != NULL) {
        free((*tree)->value);
        (*tree)->value = NULL;

        (*tree)->left = NULL;
        (*tree)->right = NULL;

        // Usuwa drzewo wewnątrz danego wierzchołka.
        if ((*tree)->inner_tree != NULL) {
            delete_whole_tree(&((*tree)->inner_tree));
        }

        free(*tree);
        *tree = NULL;
    }
}


static void swap_root_with_minimum(Avl *tree) {
    Avl minimum = (*tree)->right;
    Avl parent = (*tree);
    while (minimum->left != NULL) {
        parent = minimum;
        minimum = minimum->left;
    }

    parent->left = *tree;
    minimum->left = (*tree)->left;
    (*tree)->left = (*tree)->right;
    (*tree)->right = minimum->right;
    minimum->right = (*tree)->left;
    (*tree)->left = NULL;
    *tree = minimum;
}


static void delete_root_of_tree(Avl *tree, char *value) {
    if ((*tree)->left == NULL && (*tree)->right == NULL) {
        delete_node(tree);
    }
    // Jeśli drzewo ma tylko jednego syna, wówczas ten syn staje się korzeniem.
    else if ((*tree)->left == NULL) {
        Avl temp = *tree;
        *tree = temp->right;
        delete_node(&temp);
    }
    else if ((*tree)->right == NULL) {
        Avl temp = *tree;
        *tree = temp->left;
        delete_node(&temp);
    }
    // Jeśli prawy (lewy) syn drzewa wskazywanego przez [tree] nie ma lewego
    // (prawego) syna, wówczas jego lewym (prawym) synem zostaje lewy (prawy)
    // syn drzewa [tree], a on sam zostaje korzeniem.
    else if ((*tree)->right->left == NULL) {
        Avl temp = *tree;
        (*tree)->right->left = (*tree)->left;
        *tree = (*tree)->right;
        delete_node(&temp);
    }
    else if ((*tree)->left->right == NULL) {
        Avl temp = *tree;
        (*tree)->left->right = (*tree)->right;
        *tree = (*tree)->left;
        delete_node(&temp);
    }
    else {
        swap_root_with_minimum(tree);
        delete(&((*tree)->right), value);
    }
}


Avl exist(Avl tree, char *value) {
    if (tree == NULL || strcmp(value, tree->value) == 0) {
        return tree;
    }
    else if (strcmp(value, tree->value) < 0) {
        return exist(tree->left, value);
    }
    else {
        return exist(tree->right, value);
    }
}


void print_lexicographically(Avl tree) { //tree != NULL
    if (tree->left != NULL) {
        print_lexicographically(tree->left);
    }

    printf("%s\n", tree->value);

    if (tree->right != NULL) {
        print_lexicographically(tree->right);
    }
}


void insert(Avl *tree, char *value, Avl *world) {
    unsigned long string_length = strlen(value);
    if (*tree == NULL) {
        *tree = create_new_node(value, string_length);
    }
    else {
        if (exist(*tree, value) == NULL) {
            // Wartość dodawanego węzła jest mniejsza od wartości w korzeniu.
            if (strcmp(value, (*tree)->value) < 0) {
                if ((*tree)->left == NULL) {
                    // Kiedy dojdzie do liścia, tworzy tam nowy wierzchołek.
                    (*tree)->left =
                            create_new_node(value, string_length);
                }
                else {
                    insert(&((*tree)->left), value, world);
                }
            }
            // Wartość dodawanego węzła jest większa od wartości w korzeniu.
            else {
                if ((*tree)->right == NULL) {
                    // Kiedy dojdzie do liścia, tworzy tam nowy wierzchołek.
                    (*tree)->right =
                            create_new_node(value, string_length);
                }
                else {
                    insert(&((*tree)->right), value, world);
                }
            }

            if (*tree != NULL) {
                balance(tree);
            }
        }
    }
}


void delete_whole_tree(Avl *tree) {
    if (tree != NULL && *tree != NULL) {
        if ((*tree)->left != NULL) {
            delete_whole_tree(&(*tree)->left);
        }
        if ((*tree)->right != NULL) {
            delete_whole_tree(&(*tree)->right);
        }
        delete_node(tree);
    }
}


void delete(Avl *tree, char *value) {
    if (*tree != NULL) {
        if (strcmp(value, (*tree)->value) < 0 && (*tree)->left != NULL) {
            delete(&((*tree)->left), value);
        }
        else if (strcmp(value, (*tree)->value) > 0 && (*tree)->right != NULL) {
            delete(&((*tree)->right), value);
        }
        else if (strcmp(value, (*tree)->value) == 0) {
            delete_root_of_tree(tree, value);
        }

        if (*tree != NULL) {
            balance(tree);
        }
    }
}