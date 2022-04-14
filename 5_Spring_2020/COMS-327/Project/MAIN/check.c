#include <stdio.h>

#include "../MODULES/game/game.h"
#include "../MODULES/io/scanner.h"
#include "../MODULES/io/output.h"

/**
 * Main method for reading a game of Klondike/Solitaire and checking its validity
 * @param argc
 * number of args
 * @param argv
 * arguments
 * @return
 * 0 for success, other for failure
 */
int main(int argc, char* argv[]) {
    FILE* input = stdin;
    FILE* output = stdout;

    // Check parameters given
    switch (argc) {
        // Parameter given, attempt to read file
        case 2:
            input = fopen(argv[1], "r");
        // No parameters given, read from stdin
        case 1:
            //file exist to fopen
            if (input != NULL) {
                // Create game, validates it, and set its output
                struct game *g = inputParser(input);
                g->output->type = STATS;
                g->output->output = output;

                // If it survives validation print stats
                fprintf(output, "Input file is valid\n");
                print(*g);

                // Free memory used by the game and close the output stream
                freeGame(g);
                if (output != stdout) fclose(output);

                return 0;
            }
        default:
            // Print possible errors and stop
            fprintf(stderr, argc > 2 ? "TOO MANY ARGUMENTS PASSED\n" : "ERROR, FILE NOT FOUND!\n");
            return -1;
    }
}
