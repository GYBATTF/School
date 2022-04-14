#include <iostream>

#include "solve.hh"

solver::solver() {
    //timer = 0;
    mlimit = 1000;
    v = false;
    a = false;
    useCache = false;
    forced = false;
    attempts = 0;
    moves = NULL;
    moveIndex = 0;
}

void solver::verbose() {
    v = !v;
}

void solver::all() {
    a = !a;
}

void solver::forcedFoundations() {
    forced = !forced;
}

void solver::cache() {
    useCache = !useCache;
}

void solver::moveLimit(unsigned limit) {
    mlimit = limit;
}

void solver::solve(FILE* input) {
    moves = (struct move*) malloc(sizeof(struct move) * mlimit);
    for (int i = 0; i < mlimit - 1; i++) moves[i].next = moves + i + 1;
    moves[mlimit - 1].next = NULL;

    initial = inputParser(input);
    initial->output->type = HUMAN_READABLE;
    initial->output->output = stdout;
    doAllMoves(initial);

    if (useCache) {
        cacheSolver();
    } else {
        findSolution(initial, 0);
    } 
    // If it gets here it did not meet the exit condition 
    // and met the timer causing the recursion to bottom out
    std::cout << "# Game is not winnable within " << mlimit << " moves\n";
    freeGame(initial);
    exit(0);
}

void solver::findSolution(struct game *g, unsigned timer) {
    checkwin(g, timer);
    if (timer == mlimit) return;
    findForcedFoundations(g, timer);

    const char from[] = {'1', '2', '3', '4', '5', '6', '7', 'w', '.', 'r'};
    const char to[] = {'f', '1', '2', '3', '4', '5', '6', '7'};

    for (int i = 0; i < 10; i++) {
        if (from[i] == '.' || from[i] == 'r') {
            if (g->stock[0][0] == '\0' && g->waste[0][0] != '\0') {
                attempt(g, timer, 'r', '\0');
            } else if (g->stock[0][0] != '\0') {
                attempt(g, timer, '.', '\0');
            }
        } else {
            for (int j = 0; j < 8; j++) {
                // Keep kings from being moved unneccessarily
                if (from[i] != 'w' && to[j] != 'f') {
                    if (g->tabUncovered[i][0][0] == 'K' && g->tabCovered[i][0][0] == '\0') {
                        break;
                    }
                }
                attempt(g, timer, from[i], to[j]);
            }
        }
    }
}

void solver::findForcedFoundations(struct game *g, unsigned timer) {
    if (!forced) return;

    if (g->limit == 1) {
        attempt(g, timer, 'w', 'f');
    }

    for (int i = 0; i < 6; i++) {
        int lastCard = sizeOfPile(*g->tabUncovered[i], MAX_UNCOVERED);
        int found[4] = {g->foundations[0], g->foundations[1], g->foundations[2], g->foundations[3]};
        switch (getSuit(g->tabUncovered[i][lastCard - 1][1])) {
            case CLUBS:
                if (found[0] >= found[1] && found[0] >= found[2]) {
                    attempt(g, timer, '1' + i, 'f');
                }
                continue;
            case SPADES:
                if (found[3] >= found[1] && found[3] >= found[2]) {
                    attempt(g, timer, '1' + i, 'f');
                }
                continue;
            case DIAMONDS:
                if (found[1] >= found[0] && found[1] >= found[3]) {
                    attempt(g, timer, '1' + i, 'f');
                }
                continue;
            case HEARTS:
                if (found[2] >= found[0] && found[2] >= found[3]) {
                    attempt(g, timer, '1' + i, 'f');
                }
                continue;
        }
    }
}

void solver::attempt(struct game *g, unsigned timer, char src, char dest) {
    // Can't move to the same place
    if (src == dest) return;
    // Can't flip from an empty stock
    if (g->stock[0][0] == '\0' && src == '.') return;
    // Can't reset an empty waste
    if (g->waste[0][0] == '\0' && src == 'r') return;
    // Can't move from an empty waste
    if (g->waste[0][0] == '\0' && src == 'w') return;
    if (moveIndex > 0) {
        // Can't reset twice
        if (moves[moveIndex - 1].type == RESET && src == 'r') return;
        // Don't want to bother with repetative and unneccesary back and forths
        if (moves[moveIndex - 1].type == MOVE) {
            if (moves[moveIndex - 1].src == dest && moves[moveIndex - 1].dest == src) return;
        }
    }

    attempts++;
    if (v && dest != '\0') {
        if(attempts % 1000 == 0 || a) 
        {
            fprintf(stderr, "ATTEMPTS: %lld | DEPTH: %d | MOVE: %c->%c\n", attempts, timer, src, dest);
        }
    }
    else if (v) {
         if(attempts % 1000 == 0 || a) {
             fprintf(stderr, "ATTEMPTS: %lld | DEPTH: %d | MOVE: %s\n", attempts, timer, src == '.' ? "flip" : "reset");
             }
    }
    struct game t;
    memcpy(&t, g, sizeof(struct game));
    addMove(&t, src, dest);
    if (playMove(&t, 0)) {
        switch (src) {
            case '.':
                moves[moveIndex].type = FLIP;
                break;
            case 'r':
                moves[moveIndex].type = RESET;
                break;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case 'w':
                switch (dest) {
                    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case 'f':
                        moves[moveIndex].type = MOVE;
                        char why[2] = {'\0', '\0'};
                        why[0] = src;
                        if (src == 'w') moves[moveIndex].src = 7;
                        else moves[moveIndex].src = (int) strtol(why, NULL, 10) - 1;
                        why[0] = dest;
                        if (dest == 'f') moves[moveIndex].dest = 7;
                        else moves[moveIndex].dest = (int) strtol(why, NULL, 10) - 1;
                }
        }

        moveIndex++;
        findSolution(&t, timer + 1);
        moveIndex--;
    }
}

void solver::checkwin(struct game *g, unsigned timer) {
    if (g->stock[0][0]) return;
    if (g->waste[1][0]) return;
    for (int i = 0; i < 7; i++) if (g->tabCovered[i][0][0]) return;
    
    std::cout << "Winnable in " << timer << " moves\n";
    struct game *print = (struct game*) malloc(sizeof(struct game));
    print->moves = moves;
    moves[moveIndex - 1].next = NULL;
    moves->tail = moves + moveIndex - 1;
    printMoves(*print);
    free(print);
    freeGame(initial);
    exit(0);
}

void solver::cacheSolver() {
    // TODO implement cache solver
    std::cerr << "ERROR, CACHE SOLVER NOT IMPLEMENTED\n";
    exit(-1);
}