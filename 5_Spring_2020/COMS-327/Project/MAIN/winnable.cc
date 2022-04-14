#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../MODULES/moves/solve.hh"

int main(int argc, char* argv[]) {
    FILE* input = stdin;
    solver s = solver();

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-v")) {
            s.verbose();
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-f")) {
            s.forcedFoundations();
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-c")) {
            s.cache();
            argv[i] = NULL;
            break;
        }
    }

    // Debug option, print all moves instead of every 100 moves
    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-a")) {
            s.all();
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-m")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL) break;
            int atmp = (int) strtol(argv[i], NULL, 10);
            if (argv[i][0] == '-' && atmp != 0) {
                printf("ERROR, NEGATIVE VALUE GIVEN FOR MOVES TO ATTEMPT\n");
                exit(-1);
            } else if (strchr("0123456789", argv[i][0])) {
                s.moveLimit(atmp);
                argv[i] = NULL;
            }
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && argv[i][0] == '-') {
            fprintf(stderr, "ERROR, COMMAND LINE OPTION %s IS INVALID\n", argv[i]);
            exit(-1);
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL) {
            // Close a file if there's already an opened one
            if (input != stdin) fclose(input);
            if (!(input = fopen(argv[i], "r"))) {
                // Print error and stop
                fprintf(stderr, "ERROR, INPUT FILE '%s' NOT CANNOT BE OPENED!\n", argv[i]);
                return -1;
            }
        }
    }

    s.solve(input);
    return -1;
}