#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "../MODULES/io/interface.hh"

int main(int argc, char* argv[]) {
    interface inter;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-1")) {
            inter.turn1();
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-3")) {
            inter.turn3();
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-l")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL) {
                printf("ERROR, NO LIMIT SPECIFIED\n");
                exit(-1);
            }
            int atmp = (int) strtol(argv[i], NULL, 10);
            if (argv[i][0] == '-' && (atmp < 0 || atmp > 9)) {
                printf("ERROR, LIMIT MUST BE BETWEEN 0 AND 9\n");
                exit(-1);
            } else if (strchr("0123456789", argv[i][0])) {
                inter.limit(atmp);
                argv[i] = NULL;
            }
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-s")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL) {
                printf("ERROR, NO SEED SPECIFIED\n");
                exit(-1);
            }
            int atmp = (int) strtol(argv[i], NULL, 10);
            if (argv[i][0] == '-' && atmp != 0) {
                printf("ERROR, NEGATIVE VALUE GIVEN FOR SEED\n");
                exit(-1);
            } else if (strchr("0123456789", argv[i][0])) {
                inter.seed(atmp);
                argv[i] = NULL;
            }
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-f")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL) {
                printf("ERROR, NO FILE SPECIFIED\n");
                exit(-1);
            }
            inter.file(argv[i]);
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && argv[i][0] == '-') {
            fprintf(stderr, "ERROR, COMMAND LINE OPTION %s IS INVALID\n", argv[i]);
            exit(-1);
        }
    }

    inter.start();
    return 0;
}