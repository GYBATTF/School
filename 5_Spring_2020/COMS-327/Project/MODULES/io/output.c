#include "output.h"
#include "../moves/moves.h"
#include "../game/card.h"

/**
 * Prints the stats for a game in the format:
 * Input file is valid
 * # covered cards
 * # stock cards
 * # waste cards
 * @param g
 * @param out
 * where to output to
 */
void printStats(struct game g, FILE* out) {
    int c = 0;
    for (int i = 0; i < 7; i++) for (int j = 0; g.tabCovered[i][j][0] != '\0'; j++, c++);
    fprintf(out, "%d covered cards\n", c);

    int s = 0;
    for (; g.stock[s][0] != '\0'; s++);
    fprintf(out, "%d stock cards\n", s);

    int w = 0;
    for (; g.waste[w][0] != '\0'; w++);
    fprintf(out, "%d waste cards\n", w);
}

void printMoves(struct game g) {
    FILE* out = g.output->output;
    struct move *m = g.moves;
    int i = 0;
    if (m != 0) {
        while (m != 0) {
            int src = m->src;
            int dest = m->dest;
            switch (m->type) {
                case RESET:
                    fprintf(out, "%c    ", RESET);
                    break;
                case FLIP:
                    fprintf(out, "%c    ", FLIP);
                    break;
                case MOVE:
                    fprintf(out, "%c->%c ", src == 7 ? 'w' : src + '1', dest == 7 ? 'f' : dest + '1');
            }

            if (!(++i % 10)) fprintf(out, "\n");

            m = m->next;
        }
        fprintf(out, "\n");
    }
}

/**
 * Print the game in a way that can be read as input for this program in the format:
 * RULES:
 * turn 1
 * unlimited
 * FOUNDATIONS:
 * _c _d _h _S
 * TABLEAU:
 * cc cc | cc cc
 * cc | cc
 * | cc cc
 * cc | cc cc
 * cc | cc
 * | cc
 * |
 * STOCK:
 * cc cc | cc cc
 * MOVES:
 * #->#
 * .
 * r
 * @param g
 * game to print
 * @param out
 * where to output to
 */
void printExchange(struct game g, FILE* out) {
    // Rules
    fprintf(out, "%s\n", SECTIONS[0]);
    fprintf(out, "turn %d\n", g.turn);
    if (g.limit == -1) {
        fprintf(out, "unlimited\n");
    } else {
        fprintf(out, "limit %d\n", g.limit);
    }

    // Foundations
    fprintf(out, "%s\n", SECTIONS[1]);
    for (int i = 0; i < 4; i++) fprintf(out, "%c%c\n", getCardValueChar(g.foundations[i]), getSuitChar(i));
    fprintf(out, "\b\n");

    // Tableau
    fprintf(out, "%s\n", SECTIONS[2]);
    for (int i = 6; i >= 0; i--) {
        for (int j = 0; j < 6 && g.tabCovered[i][j][0] != '\0'; j++) fprintf(out, "%c%c ", g.tabCovered[i][j][0], g.tabCovered[i][j][1]);
        fprintf(out, "|  ");
        for (int j = 0; j < 13 && g.tabUncovered[i][j][0] != '\0'; j++) fprintf(out, "%c%c ", g.tabUncovered[i][j][0], g.tabUncovered[i][j][1]);
        fprintf(out, "\b\n");
    }

    // Stock
    fprintf(out, "%s\n", SECTIONS[3]);
    for (int i = 0; i < 24 && g.waste[i][0] != '\0'; i++) fprintf(out, "%c%c ", g.waste[i][0], g.waste[i][1]);
    fprintf(out, "|");
    for (int i = 0; i < 24 && g.stock[i][0] != '\0'; i++) fprintf(out, " %c%c", g.stock[i][0], g.stock[i][1]);
    fprintf(out, "\b\n");

    // Moves
    fprintf(out, "%s\n", SECTIONS[4]);
    printMoves(g);
}

/**
 * Print the game in a human readable format:
 * Foundations
 * _c _d _h _s
 * Tableau
 * cc ## ## ## ## ## ##
 * .. cc ## ## ## ## ##
 * .. .. cc ## ## ## ##
 * .. .. .. cc ## ## ##
 * .. .. .. .. cc ## ##
 * .. .. .. .. .. cc ##
 * .. .. .. .. .. .. cc
 * Waste top
 * cc cc cc
 * @param g
 * game to print
 * @param out
 * where to print to
 */
void printReadable(struct game g, FILE* out) {
    // Foundations
    fprintf(out, "Foundations\n");
    for (int i = 0; i < 4; i++) fprintf(out, "%c%c ", getCardValueChar(g.foundations[i]), getSuitChar(i));
    fprintf(out, "\b\n");

    // Tableau
    fprintf(out, "Tableau\n");

    char tab[7][19][2];
    for (int i = 0; i < 7; i++) for (int j = 0; j < 19; j++) for (int k = 0; k < 2; k++) tab[i][j][k] = '.';
    int t[7];
    for (int i = 0; i < 7; i++) t[i] = 0;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6 && g.tabCovered[i][j][0] != '\0'; j++, t[i]++) {
            tab[i][j][0] = tab[i][j][1] = '#';
        }
    }

    int t2[7];
    for (int i = 0; i < 7; i++) t2[i] = 0;
    for (int i = 0; i < 7; i ++) {
        for (int j = 0; j < 13 && g.tabUncovered[i][j][0] != '\0'; j++, t2[i]++) {
            tab[i][t[i] + j][0] = g.tabUncovered[i][j][0];
            tab[i][t[i] + j][1] = g.tabUncovered[i][j][1];
        }
    }

    int height;
    for (int i = height = 0; i < 7; i++) height = height < t[i] + t2[i] ? t[i] + t2[i] : height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < 7; j++) {
            fprintf(out, "%c%c ", tab[j][i][0], tab[j][i][1]);
        }
        fprintf(out, "\b\n");
    }

    // Waste
    fprintf(out, "Waste top\n");
    if (g.waste[0][0] == '\0') {
        fprintf(out, "(empty)\n");
    } else {
        int top = 0;
        while (g.waste[top][0] != '\0') top++;
        for (int i = top  - 1; i >= top - g.turn; i--) fprintf(out, "%c%c ", g.waste[i][0], g.waste[i][1]);
        fprintf(out, "\b\n");
    }
}

void print(struct game g) {
    switch(g.output->type) {
        case HUMAN_READABLE:
            printReadable(g, g.output->output);
            return;
        case STATS:
            printStats(g, g.output->output);
            return;
        case EXCHANGE:
            printExchange(g, g.output->output);
            return;
        default: return;
    }
}