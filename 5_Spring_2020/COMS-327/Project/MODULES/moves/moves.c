#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "moves.h"
#include "../game/card.h"

int sizeOfPile(char *a, int maxSize) {
    for (int i = 0; i < sizeof(char) * maxSize * 2; a++, i++) {
        if (*a == '\0') {
            return (i / (int) sizeof(char)) / 2;
        }
    }
    return maxSize;
}

/**
 * Uncovers a card on the tableau
 * @param g
 * game to uncover a card in
 */
void uncover(struct game *g) {
    for(int i = 0; i < 7; i++) {
        if (g->tabUncovered[i][0][0] != '\0') continue;
        for (int j = 0, c = sizeOfPile(*g->tabCovered[i], MAX_COVERED); c && j < 2; j++) {
            g->tabUncovered[i][0][j] = g->tabCovered[i][c - 1][j];
            g->tabCovered[i][c - 1][j] = '\0';
        }
    }
}

/**
 * Determines if a card can stack onto the given card in the tableau
 * @param src
 * card to be stacked
 * @param dest
 * card to be stacked onto, 0 for empty tableau column
 * @return
 * 1 for stackable, 0 for unstackable
 */
int canStack(char *src, char *dest) {
    if (dest == 0 && src[0] == 'K') return 1;
    if (dest == 0 || src == 0) return 0;
    if (SUIT_COLORS[getSuit(src[1])] != SUIT_COLORS[getSuit(dest[1])] && getCardValue(src[0]) + 1 == getCardValue(dest[0])) return 1;
    return 0;
}

/**
 * Moves piles around in the game
 * @param g
 * game to make move on
 */
int move(struct game *g) {
    int src = g->moves->src;
    int dest = g->moves->dest;
    int waste = sizeOfPile(*g->waste, MAX_WASTE) - 1;
    // From waste to foundation
    if (src + dest == 14) {
        int wasteSuit = getSuit(g->waste[waste][1]);
        if (g->foundations[wasteSuit] == getCardValue(g->waste[waste][0]) - 1) {
            g->foundations[wasteSuit]++;
            g->waste[waste][0] = g->waste[waste][1] = '\0';
            return 0;
        }
    // From waste to tableau
    } else if (src == 7) {
        int tSize = sizeOfPile(*g->tabUncovered[dest], MAX_UNCOVERED);
        if (canStack(g->waste[waste], tSize == 0 ? 0 : g->tabUncovered[dest][tSize - 1])) {
            for (int i = 0; i < 2; i++) {
                g->tabUncovered[dest][tSize][i] = g->waste[waste][i];
                g->waste[waste][i] = '\0';
            }
            uncover(g);
            return 0;
        }
    // From tableau to foundation
    } else if (dest == 7) {
        int tab = sizeOfPile(*g->tabUncovered[src], MAX_UNCOVERED) - 1;
        int tabSuit = getSuit(g->tabUncovered[src][tab][1]);
        if (g->foundations[tabSuit] == getCardValue(g->tabUncovered[src][tab][0]) - 1) {
            g->foundations[tabSuit]++;
            for (int i = 0; i < 2; i++) g->tabUncovered[src][tab][i] = '\0';
            uncover(g);
            return 0;
        }
    // From foundation to foundation
    } else {
        int tSrcSize = sizeOfPile(*g->tabUncovered[src], MAX_UNCOVERED);
        int tDestSize = sizeOfPile(*g->tabUncovered[dest], MAX_UNCOVERED);
        if (canStack(g->tabUncovered[src][0], tDestSize == 0 ? 0 : g->tabUncovered[dest][tDestSize - 1])) {
            for (int i = 0, j = tDestSize; i < tSrcSize; i++, j++) for (int k = 0; k < 2; k++) {
                g->tabUncovered[dest][j][k] = g->tabUncovered[src][i][k];
                g->tabUncovered[src][i][k] = '\0';
            }
            uncover(g);
            return 0;
        }
    }
    return 1;
}

/**
 * Flips the stock into the waste
 * @param g
 * game to make move on
 */
int flip(struct game *g) {

    int stockSize = sizeOfPile(*g->stock, MAX_WASTE);
    int wasteSize = sizeOfPile(*g->waste, MAX_WASTE);

    if (stockSize == 0) {
        return 1;
    }

    int mv;
    if (g->turn == 1) {
        mv = 1;
        for (int i = 0; i < 2; i++) {
            g->waste[wasteSize][i] = g->stock[0][i];
            g->stock[0][i] = '\0';
        }
    } else {
        mv = stockSize < 3 ? stockSize : 3;
        for (int j = mv, k = 0; k < j; k++, wasteSize++) {
            for (int i = 0; i < 2; i++) {
                g->waste[wasteSize][i] = g->stock[k][i];
                g->stock[k][i] = '\0';
            }
        }
    }

    for (int i = 0; i < stockSize; i++)  for (int j = 0; j < 2; j++) {
        if (i + mv < stockSize) {
            g->stock[i][j] = g->stock[i + mv][j];
        } else {
            g->stock[i][j] = '\0';
        }
    }

    return 0;
}

/**
 * Resets the waste pile back onto the stock
 * @param g
 * game to use move on
 */
int reset(struct game *g) {
    int sizeStock = sizeOfPile(*g->stock, MAX_WASTE);
    if (!sizeStock) {
        if (g->limit > 0) g->limit--;
    else if (g->limit != -1) return 1;
        int sizeWaste = sizeOfPile(*g->waste, MAX_WASTE);

        for (int i = 0; i < sizeWaste; i++) {
            for (int j = 0; j < 2; j++) {
                g->stock[i][j] = g->waste[i][j];
                g->waste[i][j] = '\0';
            }
        }

        return 0;
    } else {
        return 1;
    }
}

int playMove(struct game *g, int hardFail) {
    g->moveCounter++;
    int fail = 1;
    char *m = calloc(sizeof(char), 4);
    struct game *bk = malloc(sizeof(struct game));
    if (m == 0 || bk == 0) {
        fprintf(stderr, "ERROR, FAILED TO ALLOCATE MEMORY IN 'moves.c'\n");
        if (m != 0) free(m);
        if (bk != 0) free(bk);
        freeGame(g);
        exit(-1);
    }
    memcpy(bk, g, sizeof(struct game));

    switch (g->moves->type) {
        case RESET:
            fail = reset(bk);
            m[0] = 'r';
            break;
        case FLIP:
            fail = flip(bk);
            m[0] = '.';
            break;
        case MOVE:
            fail = move(bk);
            m[0] = "1234567w"[g->moves->src];
            m[1] = '-';
            m[2] = '>';
            m[3] = "1234567f"[g->moves->dest];
            break;
    }

    if (fail && hardFail) {
        fprintf(stderr, "Move %d is illegal: %s\n", g->moveCounter, m);
        print(*bk);
        free(m);
        free(bk);
        freeGame(g);
        exit(-1);
    } else if (!fail) {
        memcpy(g, bk, sizeof(struct game));
    }
    disconnect(g);
    free(m);
    free(bk);
    return !fail;
}

void doAllMoves(struct game *g) {
    while (g->moves != 0) {
        playMove(g, 1);
    }
}

void doMoves(struct game *g, int n) {
    int i = 0;
    for (;(i < n || n == -1) && g->moves != 0; i++) {
        playMove(g, 1);
    }
}

void disconnect(struct game *g) {
    if (g->moves->next != 0) g->moves->next->tail = g->moves->tail;
    struct move *m = g->moves;
    g->moves = g->moves->next;
    free(m);
}

/**
 * Connects two nodes together
 * @param g
 * game to connect node to
 * @param m
 * move to connect
 */
void connect(struct game *g, struct move *new) {
    struct move *m = malloc(sizeof(struct move));
    memcpy(m, new, sizeof(struct move));
    m->next = 0;

    if (!g->moves) {
        g->moves = m;
    } else {
        m->tail = g->moves->tail;
        g->moves->tail->next = m;
    }

    g->moves->tail = m;
}

void addMove(struct game *g, char c, char d) {
    struct move m;
    switch (c) {
        case '.':
            m.type = FLIP;
            connect(g, &m);
            return;
        case 'r':
            m.type = RESET;
            connect(g, &m);
            return;
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case 'w':
            switch (d) {
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case 'f':
                    m.type = MOVE;
                    char why[2] = {'\0', '\0'};
                    why[0] = c;
                    if (c == 'w') m.src = 7;
                    else m.src = (int) strtol(why, NULL, 10) - 1;
                    why[0] = d;
                    if (d == 'f') m.dest = 7;
                    else m.dest = (int) strtol(why, NULL, 10) - 1;
                    connect(g, &m);
                    return;
                default: break;
            }
        default:
            fprintf(stderr, "ERROR, MOVE SRC %c OR MOVE DEST %c IS INVALID\n", c, d);
            exit(-1);
    }
}