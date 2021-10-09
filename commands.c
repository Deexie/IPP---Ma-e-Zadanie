#include "commands.h"

/*
 * Zwraca 0, jeśli zwierzę jest nieokreślone lub jeśli dane zwierzę występuje
 * w [root]. W przeciwnym przypadku zwraca 1.
 */
static int check_animal(struct nature creation, Avl root);

/*
 * Sprawdza, czy dane drzewo wraz ze znajdującym się w nim zwierzęciem występuje
 * w danym lesie [root]. Jeśli w [creation] drzewo jest określone symbolem ‘*’,
 * wówczas sprawdzane są wszystkie drzewa w danym lesie. Zwraca 0, jeśli drzewo
 * jest nieokreślone lub jeśli dane drzewo i znajdujące się na nim dane zwierzę
 * występuje w [root]. W przeciwnym przypadku zwraca 1.
 */
static int check_tree(struct nature creation, Avl root);

/*
 * Sprawdza, czy dany las wraz ze znajdującym się w nim drzewem i zwierzęciem
 * występuje w [root]. Jeśli w [creation] las jest określony symbolem ‘*’,
 * wówczas sprawdzane są wszystkie lasy. Zwraca 0, jeśli dana formacja występuje
 * w [root]. W przeciwnym przypadku zwraca 1.
 */
static int check_forest(struct nature creation, Avl root);



static int check_animal(struct nature creation, Avl root) {
    if (creation.animal == NULL) {
        return 0;
    }
    else {
        return (exist(root, creation.animal) == NULL);
    }
}


static int check_tree(struct nature creation, Avl root) {
    if (creation.tree == NULL) {
        return 0;
    }
    else {
        if (strcmp(creation.tree, "*") == 0) {
            if (root == NULL) {
                return 1;
            }
            else {
                // Sprawdza kolejno, czy w korzeniu drzewa [root] znajduje się
                // określone zwierzę, czy dane zwierzę znajduje się w lewym
                // poddrzewie i czy znajduje się w prawym poddrzewie drzewa
                // [root]. Jeśli któraś z tych opcji zostanie spełniona
                // (przyjmie wartość 0), wówczas kolejne nie są już sprawdzane.
                int animals_on_this_tree =
                        check_animal(creation, root->inner_tree);
                int not_exist = animals_on_this_tree
                                && check_tree(creation, root->left)
                                && check_tree(creation, root->right);
                return not_exist;
            }
        }
        else {
            if (exist(root, creation.tree) != NULL) {
                Avl tree_to_check = exist(root, creation.tree)->inner_tree;
                return check_animal(creation, tree_to_check);
            }
            else {
                // Dane drzewo nie istnieje.
                return 1;
            }
        }
    }
}


static int check_forest(struct nature creation, Avl root) {
    if (strcmp(creation.forest, "*") == 0) {
        if (root == NULL) {
            return 1;
        }
        else {
            // Sprawdza kolejno, czy w korzeniu [root] znajduje się określone
            // drzewo, czy dane drzewo znajduje się w lewym poddrzewie i czy
            // znajduje się w prawym poddrzewie [root]. Jeśli któraś z tych
            // opcji zostanie spełniona (przyjmie wartość 0), wówczas kolejne
            // nie są już sprawdzane.
            int trees_in_this_forest =
                    check_tree(creation, root->inner_tree);
            int not_exist = trees_in_this_forest
                            && check_forest(creation, root->left)
                            && check_forest(creation, root->right);
            return not_exist;
        }
    }
    else {
        if (exist(root, creation.forest) != NULL) {
            Avl forest_to_check = exist(root, creation.forest)->inner_tree;
            return check_tree(creation, forest_to_check);
        }
        else {
            // Dany las nie istnieje.
            return 1;
        }
    }
}


void add(struct nature new_creation, Avl *root) {
    // Dodaje las
    insert(root, new_creation.forest, root);

    if (new_creation.tree != NULL) {
        Avl new_forest = exist(*root, new_creation.forest);
        // Dodaje drzewo
        insert(&(new_forest->inner_tree), new_creation.tree, root);

        if (new_creation.animal != NULL) {
            Avl new_tree = exist(new_forest->inner_tree, new_creation.tree);
            // Dodaje zwierzę
            insert(&(new_tree->inner_tree), new_creation.animal, root);
        }
    }
}


void print(struct nature creation, Avl root) {
    Avl formation_to_print;

    // Jeśli [forest] (a więc również wszystkie wskaźniki) w strukturze
    // [creation] jest równy NULL, wówczas wypisane zostaną wszystkie lasy.
    if (creation.forest == NULL) {
        formation_to_print = root;
    }
    else {
        // Dany las nie istnieje, więc nic nie zostanie wypisane.
        if (exist(root, creation.forest) == NULL) {
            formation_to_print = NULL;
        }
        else {
            Avl node_to_print = exist(root, creation.forest);
            formation_to_print = (node_to_print)->inner_tree;

            if (creation.tree != NULL) {
                // Dane drzewo nie istnieje w danym lesie, więc nic nie
                // zostanie wypisane.
                if (exist(formation_to_print, creation.tree) == NULL) {
                    formation_to_print = NULL;
                }
                else {
                    node_to_print = exist(formation_to_print, creation.tree);
                    formation_to_print = (node_to_print)->inner_tree;
                }
            }
        }
    }

    if (formation_to_print != NULL) {
        print_lexicographically(formation_to_print);
    }
}


void del(struct nature creation, Avl *root) {
    if (root == NULL) {
        return;
    }

    if (creation.forest == NULL) {
        delete_whole_tree(root);
    }
    else if (creation.tree == NULL) {
        delete(root, creation.forest);
    }
    else {
        Avl temp_forest = exist(*root, creation.forest);

        // Sprawdza, czy las o danej nazwie istnieje i czy zawiera jakieś
        // drzewa.
        if (temp_forest != NULL && temp_forest->inner_tree != NULL) {
            if (creation.animal == NULL) {
                delete(&(temp_forest->inner_tree), creation.tree);
            }
            else {
                Avl temp_tree = exist(temp_forest->inner_tree, creation.tree);

                // Sprawdza, czy drzewo o danej nazwie istnieje w danym
                // lesie i czy zawiera jakieś zwierzęta.
                if (temp_tree != NULL && temp_tree->inner_tree != NULL) {
                    delete(&(temp_tree->inner_tree), creation.animal);
                }
            }
        }
    }
}


void check(struct nature creation, Avl root) {
    if (check_forest(creation, root) == 0) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}