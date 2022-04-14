#include <stdlib.h>

#include "game.h"
#include "card.h"
#include "../io/validator.h"
#include "../moves/moves.h"


struct game* getGame() {
    struct game *g = calloc(sizeof(struct game), 1);
    g->turn = -1;
    g->limit = -1;
    g->moveCounter = 0;
    g->output = calloc(sizeof(struct out), 1);
    return g;
}

/**
 * frees the moves list
 * @param g
 * game to free from
 */
void freeMoves(struct game *g) {
    if (g->moves == 0) return;
    struct move *m = g->moves;
    while (m) {
        struct move *n = m->next;
        free(m);
        m = n;
    }
}

void freeGame(struct game *g) {
    freeMoves(g);
    free(g->output);
}

struct game* getRandomGame(unsigned long seed, int limit, int turn) {
    unsigned cards = 53;
    char deck[52][2];
    for (int suit = 0; suit < 4; suit++) {
        for (int value = 1; value < 14; value++) {
            deck[(13 * suit) + (value - 1)][0] = getCardValueChar(value);
            deck[(13 * suit) + (value - 1)][1] = getSuitChar(suit);
        }
    }

    struct game *g = getGame();
    g->limit = limit;
    g->turn = turn;
    srandom(seed);

    int stock = 0;
    int covered = 0;
    int tab = 0;
    while (--cards > 0) {
        int pick;
        if (cards != 0) pick = random() % cards;
        else pick = 0;

        if (cards > 28) {
            g->stock[stock][0] = deck[pick][0];
            g->stock[stock++][1] = deck[pick][1];
        } else if (cards > 27) {
            g->tabUncovered[tab][0][0] = deck[pick][0];
            g->tabUncovered[tab++][0][1] = deck[pick][1];
        } else if (cards > 25) {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                covered = 0;
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab++][0][1] = deck[pick][1];
            }
        } else if (cards > 22) {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                covered = 0;
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab++][0][1] = deck[pick][1];
            }
        } else if (cards > 18) {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                covered = 0;
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab++][0][1] = deck[pick][1];
            }
        } else if (cards > 13) {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                covered = 0;
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab++][0][1] = deck[pick][1];
            }
        } else if (cards > 7) {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                covered = 0;
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab++][0][1] = deck[pick][1];
            }
        } else {
            if (covered < tab) {
                g->tabCovered[tab][covered][0] = deck[pick][0];
                g->tabCovered[tab][covered++][1] = deck[pick][1];
            } else {
                g->tabUncovered[tab][0][0] = deck[pick][0];
                g->tabUncovered[tab][0][1] = deck[pick][1];
            }
        }

        while (pick < cards) {
            deck[pick][0] = deck[pick + 1][0];
            deck[pick][1] = deck[pick + 1][1];
            pick++;
        }
    }
    
    return g;
}