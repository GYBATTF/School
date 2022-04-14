#ifndef PART1_GAME_H
#define PART1_GAME_H

#include "../io/output.h"

#define MAX_WASTE 24
#define MAX_STOCK 24
#define MAX_UNCOVERED 13
#define MAX_COVERED 6

/**
 * Stores the state of a game of Klondike/Solitaire
 */
struct game {
    // Covered cards in the tableau
    char tabCovered[7][MAX_COVERED][2];
    // Uncovered cards in the tableau
    char tabUncovered[7][MAX_UNCOVERED][2];
    // Foundations set
    // {c, d, h, s}
    int foundations[4];
    // Cards in the stock
    char stock[MAX_STOCK][2];
    // Cards in the waste
    char waste[MAX_WASTE][2];
    // How many cards to flip
    int turn;
    // Limit of flips, -1 for unlimited
    int limit;
    // Array of moves
    struct move *moves;
    // Output method
    struct out *output;
    // Moves done
    int moveCounter;
};

/**
 * Initialized a game struct's elements
 * @return
 * an initialized game
 */
struct game* getGame();

struct game* getRandomGame(unsigned long seed, int limit, int turn);

/**
 * Frees the memory used by a game
 * @param g
 * game to free
 */
void freeGame(struct game *g);

#endif //PART1_GAME_H
