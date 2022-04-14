#ifndef COMS_327_GROUP_PROJECT_OUTPUT_H
#define COMS_327_GROUP_PROJECT_OUTPUT_H

#include <stdio.h>

// For some reason scanner has to be out here
#include "scanner.h"
#include "../game/game.h"

enum TYPES {EXCHANGE = 'x', HUMAN_READABLE = 'h', STATS = 's'};

struct out {
    FILE* output;
    enum TYPES type;
};

/**
 * Prints out a game in the format specified in its output type
 * @param g
 * game to print
 */
void print(struct game g);

/**
 * Prints out the moves in a game in the format specified in its output type
 * @param g
 * game to print moves from
 */
void printMoves(struct game g);

#endif //COMS_327_GROUP_PROJECT_OUTPUT_H
