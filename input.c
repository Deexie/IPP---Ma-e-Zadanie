#define _XOPEN_SOURCE 700

#include "input.h"

#define WORDS 5

static void error();

static void ok();

/*
 * Zwraca 1, gdy wyraz [string] zawiera tylko znaki rozszerzonego ASCII
 * o kodach od 33 do 255. W przeciwnym przypadku zwraca 0.
 */
static int contain_only_ascii_signs(char *string);

static int asterisk_as_last_sign(char *strings[WORDS]);

static inline int is_add_command(char *strings[WORDS]);

static inline int is_del_command(char *strings[WORDS]);

static inline int is_print_command(char *strings[WORDS]);

static inline int is_check_command(char *strings[WORDS]);

static void interpret(char *strings[WORDS], Avl *root);

/*
 * Zwraca 1, jeśli linia, z której pochodzi [token], zawiera znaki inne
 * niż białe i jest zakończona znakiem innym niż ‘\n’ lub jeśli pierwszy wyraz
 * ([token]) zawiera niedozwolone znaki. W przeciwnym przypadku zwraca 0.
 */
static int incorrect_command(char *token, int newline_as_last_sign);

/*
 * Dzieli linię tekstu [string] na wyrazy. Wyrazy mogą byc oddzielone białymi
 * znakami (znakami o kodach ASCII: 32, 9, 11, 12, 13).
 */
static void parse(char *string, Avl *root, int newline_as_last_sign);

static void get_line(char **string, int *characters);



static void error() {
    fprintf(stderr, "ERROR\n");
}


static void ok() {
    printf("OK\n");
}


static int contain_only_ascii_signs(char *string) {
    if (string != NULL) {
        for (unsigned long i = 0; i < strlen(string); i++) {
            if ((unsigned int)string[i] < 33) {
                return 0;
            }
        }
    }
    return 1;
}


static int asterisk_as_last_sign(char *strings[WORDS]) {
    int i = 1;
    while(strings[i] != NULL) {
        i++;
    }

    if (strcmp(strings[i - 1], "*") == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


static inline int is_add_command(char *strings[WORDS]) {
    return strcmp(strings[0], "ADD") == 0 && strings[1] != NULL;
}


static inline int is_del_command(char *strings[WORDS]) {
    return strcmp(strings[0], "DEL") == 0;
}


static inline int is_print_command(char *strings[WORDS]) {
    return strcmp(strings[0], "PRINT") == 0 && strings[3] == NULL;
}


static inline int is_check_command(char *strings[WORDS]) {
    int is_check = strcmp(strings[0], "CHECK") == 0;
    return is_check && strings[1] != NULL && !asterisk_as_last_sign(strings);
}


static void interpret(char *strings[WORDS], Avl *root) {
    if (strings[0] != NULL) {
        struct nature temp = {.forest = strings[1], .tree = strings[2],
                .animal = strings[3]};

        if (is_add_command(strings)) {
            add(temp, root);
            ok();
        }
        else if (is_del_command(strings)) {
            del(temp, root);
            ok();
        }
        else if (is_print_command(strings)) {
            print(temp, *root);
        }
        else if (is_check_command(strings)) {
            check(temp, *root);
        }
        else {
            error();
        }
    }
}


static int incorrect_command(char *token, int newline_as_last_sign) {
    int incorrect = !newline_as_last_sign && token != NULL;
    return incorrect || !contain_only_ascii_signs(token);
}


static void parse(char *string, Avl *root, int newline_as_last_sign) {
    char *tokens[WORDS] = {NULL, NULL, NULL, NULL, NULL};
    int i = 0;

    tokens[0] = strtok(string, " \t\v\f\r\n");

    if (incorrect_command(tokens[0], newline_as_last_sign)) {
        error();
    }
    else {
        // Z ciągu znaków [string] wydzielane jest maksymalnie 5 słów (większa
        // ich liczba oznacza, że dany ciąg jest nieprawidłowy).
        while (tokens[i] != NULL && i < WORDS - 1) {
            i++;

            tokens[i] = strtok(NULL, " \t\v\f\r\n");
            if (!contain_only_ascii_signs(tokens[i])) {
                error();
                return;
            }
        }

        // Jeśli piąte słowo jest niepuste, ciąg znaków jest nieprawidłowy.
        if (tokens[WORDS - 1] != NULL) {
            error();
            return;
        }
        else {
            interpret(tokens, root);
        }
    }
}


static void get_line(char **string, int *characters) {
    size_t size = 20 * sizeof(char);
    *string = malloc(size);
    if (*string == NULL) {
        // Nie można zaalokować pamięci.
        exit(1);
    }
    *characters = getline(string, &size, stdin);
}


void read(Avl *root) {
    char *string;
    int size = 0;

    get_line(&string, &size);
    while (size != -1) {
        if (string[0] != '#' && string[0] != '\n') {
            // Sprawdza, czy '\n' jest ostatnim znakiem
            if (string[strlen(string) - 1] == '\n') {
                parse(string, root, 1);
            }
            else {
                parse(string, root, 0);
            }
        }

        free(string);
        get_line(&string, &size);
    }
    free(string);
}