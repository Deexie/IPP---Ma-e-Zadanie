#include "input.h"

int main() {
    Avl root = NULL;
    read(&root);
    if (root != NULL) {
        delete_whole_tree(&root);
    }
    return 0;
}