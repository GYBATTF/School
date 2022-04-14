#include <stdio.h>
#include <stdlib.h>

#include "../game/card.h"
#include "../game/game.h"

/**
 * Checks the order of the card in the tableau to ensure that they are stacked correctly
 * @param g
 * Game to check
 */
void tabOrderCheck(struct game *g) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0, pCol = 0, pVal = 0, pSuit = 0, bye = 0; j < 13 && g->tabUncovered[i][j][0] != '\0'; j++) {
            if (j == 0) {
                pVal = getCardValue(g->tabUncovered[i][j][0]);
                pSuit = getSuit(g->tabUncovered[i][j][1]);
                pCol = SUIT_COLORS[pSuit];
                continue;
            } else {
                int val = getCardValue(g->tabUncovered[i][j][0]);
                int col = SUIT_COLORS[getSuit(g->tabUncovered[i][j][1])];
                if (pCol == col) {
                    fprintf(stderr, "ERROR IN TABLEAU COLUMN %d: CARDS %c%c AND %c%c ARE NOT STACKED EVERY OTHER COLOR\n",
                            i + 1, getCardValueChar(pVal), getSuitChar(pSuit), getCardValueChar(pVal), g->tabUncovered[i][j][1]);
                    bye = 1;
                }

                if (pVal <= val) {
                    fprintf(stderr, "ERROR IN TABLEAU COLUMN %d: CARDS %c%c AND %c%c ARE NOT STACKED IN DESCENDING ORDER BY VALUE\n",
                            i + 1, getCardValueChar(pVal), getSuitChar(pSuit), getCardValueChar(pVal), g->tabUncovered[i][j][1]);
                    bye = 1;
                }

                if (bye) {
                    freeGame(g);
                    exit(-1);
                }

                pVal = val;
                pSuit = getSuit(g->tabUncovered[i][j][1]);
                pCol = col;
            }

        }
    }
}

/**
 * Checks to ensure no cards are missing or duplicated
 * @param g
 * the game board
 */
void allAboard(struct game *g) {
    // Array for storing number of cards counted
    int count[4][13];
    for (int i = 0; i < 4; i++) for (int j = 0; j < 13; j++)  count[i][j] = 0;


    // Count cards
    for (int i = 0; i < 24; i++) {
        // waste
        if (g->waste[i][0] != '\0') {
            int wv = getCardValue(g->waste[i][0]);
            int ws = getSuit(g->waste[i][1]);
            count[ws][wv - 1] += 1;
        }
        // stock
        if (g->stock[i][0] != '\0') {
            int sv = getCardValue(g->stock[i][0]);
            int ss = getSuit(g->stock[i][1]);
            count[ss][sv - 1] += 1;
        }
        // tableau :(
        if (i < 7) {
            for (int j = 0, b = 1; j < 13; j++) {
                // Piggyback this for loop to count foundations
                if (i < 4 && j < g->foundations[i]) count[i][j] += 1;

                if (g->tabCovered[i][j][0] == '\0') b = 0;
                if (j < 6 && b) {
                    int tv = getCardValue(g->tabCovered[i][j][0]);
                    int ts = getSuit(g->tabCovered[i][j][1]);
                    count[ts][tv - 1] += 1;
                }
                if (g->tabUncovered[i][j][0] != '\0') {
                    int tcv = getCardValue(g->tabUncovered[i][j][0]);
                    int tcs = getSuit(g->tabUncovered[i][j][1]);
                    count[tcs][tcv - 1] += 1;
                }
            }
        }
    }

    // Check if we have the right number of cards or not and print proper error message
    // Array for storing number of cards >1
    int tooHot[4][13];
    // Array for storing number of cards <1
    int tooCold[4][13];
    for (int i = 0; i < 4; i++) for (int j = 0; j < 13; tooHot[i][j] = 0, tooCold[i][j] = 0, j++);
    // This should be ok
    for (int i = 0; i < 4; i++) for (int j = 0; j < 13; j++) {
            if (count[i][j] > 1) tooHot[i][j] = count[i][j];
            if (count[i][j] < 1) tooCold[i][j] = 1;
        }

    // Total up array, valid files have both totals =0
    int heat = 0;
    int chill = 0;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 13; heat += tooHot[i][j], chill += tooCold[i][j], j++);

    // Print errors if needed
    if (heat || chill) {
        if (heat) {
            fprintf(stderr, "TOO MANY OF: ");
            for (int i = 0; i < 4; i++) for (int j = 0; j < 13; j++) {
                    if (tooHot[i][j]) {
                        fprintf(stderr, "{%c%c : %d}, ", getCardValueChar(j + 1), getSuitChar(i), tooHot[i][j]);
                    }
                }
            fprintf(stderr, "\b\b  \n");
        }
        if (chill) {
            fprintf(stderr, "TOO FEW OF: ");
            for (int i = 0; i < 4; i++) for (int j = 0; j < 13; j++) {
                    if (tooCold[i][j]) {
                        fprintf(stderr, "%c%c, ", getCardValueChar(j + 1), getSuitChar(i));
                    }
                }
            fprintf(stderr, "\b\b  \n");
        }

        // Clean up and exit
        freeGame(g);
        exit(-1);
    }
}

void validate(struct game g) {
    allAboard(&g);
    tabOrderCheck(&g);
}
