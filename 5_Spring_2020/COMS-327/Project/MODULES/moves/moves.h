#ifndef COMS_327_GROUP_PROJECT_MOVES_H
#define COMS_327_GROUP_PROJECT_MOVES_H

#include "../game/game.h"

enum MOVE_TYPE {RESET = 'r', FLIP = '.', MOVE = '>', NOTHING = 0};

/**
 * Stores information about a move
 */
struct move {
    enum MOVE_TYPE type;
    int src;
    int dest;
    struct move *next;
    struct move *tail;
};

/**
 * Gets the number of card in a pile
 * @param a
 * pile to get number of cards in
 * @param maxSize
 * maximum size of the pile
 * @return
 * size of the pile
 */
int sizeOfPile(char *a, int maxSize);

/**
 * Adds a move to a game.
 * @param g
 * game to add move to
 * @param c
 * Valid options for c are:
 * r - reset
 * . - flip
 * [1-7w] - source position
 * @param dest
 * Valid options for dest are:
 * \0 - move is a reset or flip
 * [1-7f] - destination position
 */
void addMove(struct game *g, char c, char d);

/**
 * Removes a played move
 * @param g
 * game to remove played move from
 */
void disconnect(struct game *g);

/**
 * Does all the moves stored in the game
 * @param g
 * game to do moves from with
 */
void doAllMoves(struct game *g);

/**
 * Attempts to do the number of moves specified
 * @param g
 * game to do moves on from
 * @param n
 * number of moves to try to do. if the number is greater than the number of listed moves,
 * it will do as many as are available
 */
void doMoves(struct game *g, int n);

/**
 * Calls the proper function for the move to be played,
 * then removes the play from the game
 * @param g
 * game to make play from
 * @param hardFail
 * if true, causes the program to quit with an error message when an invalid move is processed
 * @return
 * true if successful?
 */
int playMove(struct game *g, int hardFail);

#endif //COMS_327_GROUP_PROJECT_MOVES_H
