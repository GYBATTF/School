#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../MODULES/game/game.h"
#include "../MODULES/io/scanner.h"
#include "../MODULES/moves/moves.h"
#include "../MODULES/io/output.h"

int main(int argc, char* argv[]) {
    // Init IO pointers
    FILE* input = stdin;
    FILE* output = stdout;

    // Variables for switches
    int exchange = 0;
    int moves = -1;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-x")) {
            exchange = 1;
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-m")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL) break;
            int try = (int) strtol(argv[i], NULL, 10);
            if (argv[i][0] == '-' && try != 0) {
                printf("ERROR, NEGATIVE VALUE GIVEN FOR MOVES\n");
                exit(-1);
            } else if (strchr("0123456789", argv[i][0])) {
                moves = try;
                argv[i] = NULL;
            }
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && !strcmp(argv[i], "-o")) {
            argv[i++] = NULL;
            if (i >= argc || argv[i] == NULL || argv[i][0] == '-') {
                fprintf(stderr, "ERROR, OPTION '-o' REQUIRES A PARAMETER\n");
                exit(-1);
            }
            // Close a file if there's already an opened one
            if (output != stdout) fclose(output);
            // Open a file
            if (!(output = fopen(argv[i], "w"))) {
                fprintf(stderr, "ERROR, FILE '%s' CANNOT BE OPENED FOR WRITING\n", argv[i]);
                exit(-1);
            }
            argv[i] = NULL;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL && argv[i][0] == '-') {
            fprintf(stderr, "ERROR, COMMAND LINE OPTION %s IS INVALID", argv[i]);
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

    // Get the game and validate it
    struct game *g = inputParser(input);

    // Set the output options
    g->output->type = exchange ? EXCHANGE : HUMAN_READABLE;
    g->output->output = output;

    // Do moves
    if (moves == -1) {
        doAllMoves(g);
    } else {
        doMoves(g, moves);
    }

    // Print output
    printf("Processed %d moves, all valid\n", g->moveCounter);
    print(*g);

    // Cleanup and exit
    freeGame(g);
    if (output != stdout) fclose(output);
    return 0;
}
