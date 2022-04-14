#include <stdio.h>
#include <stdlib.h>

#include "../game/game.h"
#include "../game/card.h"
#include "scanner.h"
#include "validator.h"
#include "../moves/moves.h"

const char SECTIONS[5][MAX_WORD_SIZE] = {"RULES:", "FOUNDATIONS:", "TABLEAU:", "STOCK:", "MOVES:"};

/**
 * State of input reader, kept in global scope so other methods can access its variables
 */
struct state {
    // Line number we are on, used in error messages
    int line;
    // Section of save we are reading
    int section;
    // Track if we are/have setting turn/limit. 0 = not setting, 1 = waiting for value, -1 = set, -2 = currently checking limit/unlimited
    int setTurn;
    int setLimit;
    // Track which foundations are set
    int setFoundations[4];
    // Track how much of the tableau has been set
    int tabs;
    int tabCol;
    int covUncov;
    // Track if we are adding stock or waste
    int stock;
    int stockIndex;
    int colChange;
};

/**
 * Initialized the tracker for the state of the input reading
 * @return
 * an initialized state struct
 */
struct state getState() {
    struct state s;
    s.line = 0;
    s.section = -1;
    s.setTurn = 0;
    s.setLimit = 0;
    for (int i = 0; i < 4; i++) s.setFoundations[i] = 0;
    s.tabs = 6;
    s.tabCol = 0;
    s.stock = 0;
    s.stockIndex = 0;
    s.covUncov = 0;
    s.colChange = 0;
    return s;
}

/**
 * Gets a word from the input stream
 * @param in
 * input stream to read
 * @return
 * a char[] of a word
 */
void getWord(FILE *in, char word[], struct state s) {
    // init
    for (int i = 0; i < MAX_WORD_SIZE; i++) word[i] = '\0';

    // Read chars
    char c;
    int size = 0;
    for (c = (char) fgetc(in); size < MAX_WORD_SIZE && !feof(in) && c != '\t' && c != ' ' && c != '\n'; c = (char) fgetc(in), size++) word[size] = c;
    if (c == '\n') {
        ungetc(c, in);
    }

    if (word[0] == '|' && word[1] != '\0') {
        fprintf(stderr, "ERROR ON LINE %d: '|' CHARACTER SHOULD BE SURROUNDED BY SPACES\n", s.line);
        exit(-1);
    }
}

/**
 * Compares two char arrays up to the MAX_WORD_SIZE
 * @return
 * 1 if equal
 * 0 if not equal
 */
int compare(const char o1[], const char o2[]) {
    for (int i = 0; i < MAX_WORD_SIZE; i++) {
        if (o1[i] == '\0' && o2[i] == '\0' ) {
            return 1;
        } else if (o1[i] != o2[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Validates a move from the MOVES section
 * @param move
 * move to validate
 * @return
 * 0 for invalid, 1 for valid
 */
int validateMove(const char move[]) {
    if (move[0] == '.' || move[0] == 'r') return 1;
    if (move[1] == '-' && move[2] == '>') {
        for (int i = 0, src = 0, dest = 0; i < 8; i++) {
            if ("1234567w"[i] == move[0]) src = 1;
            if ("1234567f"[i] == move[3]) dest = 1;
            if (src && dest) return 1;
        }
    }
    return 0;
}

/**
 * Checks that the order is valid and prints an error if it isn't
 * @param w
 * word to check against
 * @param s
 * state of parser
 * @param next
 * which section to test
 */
void orderCheck(char w[], struct state *s, int next) {
    for (int i = 0; i < 5; i++) {
        if (compare(w, SECTIONS[i])) {
            if (i != next) {
                fprintf(stderr, "ERROR ON LINE %d: EXPECTED '%s\b' BUT GOT '%s\b'\n", s->line, SECTIONS[next], SECTIONS[i]);
                exit(-1);
            }
        }
    }
}

/**
 * Parses the rules section
 * @param g
 * the game to add parsed rules to
 * @param s
 * state of the parser
 * @param w
 * word that was most recently grabbed
 */
void parseRules(struct game *g, struct state *s, char w[]) {
    if (s->setLimit == -1 && s->setTurn == -1) {
        fprintf(stderr, "ERROR ON LINE %d: INVALID OPTION %s FOUND IN RULES SECTION\n", s->line, w);
        exit(-1);
    }

    if (compare(w, "turn") && !(s->setTurn != 0 || s->setLimit != 0)) {
        s->setTurn = 1;
        return;
    } else if (compare(w, "turn") && !(s->setTurn == 0 || s->setLimit == 0)) {
        fprintf(stderr, "ERROR ON LINE %d: TURN HAS ALREADY BEEN SET\n", s->line);
        exit(-1);
    }

    if (s->setTurn == 1) {
        int counter = 0;
        for(int i = 0; i < MAX_WORD_SIZE; i++) {
            if(w[i] != '\0') {
                counter++;
            }
        }
        if(counter >= 2) {
            fprintf(stderr, "ERROR ON LINE %d: INVALID TURN NUMBER\n", s->line);
            exit(-1);
        } else {
            g->turn = w[0] - '0';
            if (g->turn != 1 && g->turn != 3) {
                fprintf(stderr, "ERROR ON LINE %d: INVALID TURN NUMBER %d\n", s->line, g->turn);
                exit(-1);
            }
            s->setTurn = -1;
        }

    }

    if ((compare(w, "unlimited") || compare(w, "limit")) && s->setTurn == -1) {
        if (compare(w, "unlimited")) {
            g->limit = -1;
            s->setLimit = -1;
        } else if(compare(w, "limit") && s->setLimit == 0) {
            s->setLimit = 1;
            return;
        }
    } else if ((compare(w, "unlimited") || compare(w, "limit")) && s->setTurn != -1) {
        fprintf(stderr, "ERROR ON LINE %d: TURN HAS NOT BEEN SET\n", s->line);
        exit(-1);
    }

    if(s->setLimit == 1) {
        int counter = 0;
        for(int i = 0; i < MAX_WORD_SIZE; i++) {
            if(w[i] != '\0') {
                counter++;
            }
        }

        if(counter >= 2){
            fprintf(stderr, "ERROR ON LINE %d: INVALID LIMIT NUMBER\n", s->line);
            exit(-1);
        } else {
            g->limit = w[0] - '0';
            if(g->limit < 0 || g->limit > 9)
            {
                fprintf(stderr, "ERROR ON LINE %d: INVALID LIMIT NUMBER\n", s->line);
                exit(-1);
            }
            s->setLimit = -1;
        }
    }
}

/**
 * Parses the foundations section
 * @param g
 * the game to add parsed rules to
 * @param s
 * state of the parser
 * @param w
 * word that was most recently grabbed
 */
void parseFoundations(struct game *g, struct state *s, char w[]) {
    // Get values and assign them to the game
    int v = getCardValue(w[0]);
    enum SUITS c = getSuit(w[1]);

    if (c == ERR) {
        fprintf(stderr, "ERROR ON LINE %d: INVALID SUIT TYPE '%c'\n", s->line, w[1]);
        exit(-1);
    }

    g->foundations[c] = v;
    s->setFoundations[c] = 1;

    // Check how many foundations up to and including this one have been set
    // If in proper order they should all be set
    int t = 0;
    for (int i = 0; i <= c; i++) {
        t += s->setFoundations[i];
    }

    // Check the value of the card
    if (v == -1) {
        fprintf(stderr, "ERROR ON LINE %d: VALUE '%c' IS INVALID IN FOUNDATIONS SECTION\n", s->line, w[0]);
        exit(-1);
    }

    // Check the order of the card
    if (t != (c + 1)) {
        fprintf(stderr, "ERROR ON LINE %d: '%c' DEVIATES FROM THE ORDER OF {c, d, h, s}\n", s->line, w[1]);
        exit(-1);
    }
}

/**
 * Parses the tableau section
 * @param g
 * the game to add parsed tableau to
 * @param s
 * state of the parser
 * @param w
 * word that was most recently grabbed
 */
void parseTableau(struct game *g, struct state *s, char w[]) {
    // Get the values of the card and suit
    int v = getCardValue(w[0]);
    enum SUITS c = getSuit(w[1]);

    // Check the type
    if (c == ERR && w[0] != '|') {
        fprintf(stderr, "ERROR ON LINE %d: %c IS AN INVALID CARD TYPE\n", s->line, w[1]);
        exit(-1);
    }

    // Check the value
    if (v <= 0 && w[0] != '|') {
        fprintf(stderr, "ERROR ON LINE %d: VALUE %c IS INVALID\n", s->line, w[0]);
        exit(-1);
    }

    // Decrement the row counter and reset the other variables
    if (s->colChange != s->line) {
        s->colChange = s->line;
        if (s->covUncov) {
            s->tabs--;
        }
        s->covUncov = 0;
        s->tabCol = 0;
    }

    if (s->tabs < 0) {
        fprintf(stderr, "ERROR ON LINE %d: TOO MANY TABLEAU COLUMNS PRESENT", s->line);
        exit(-1);
    }

    // Flip between covered and uncovered
    if (w[0] == '|') {
        s->tabCol = 0;
        s->covUncov = 1;
        return;
    }

    if (s->covUncov) {
        if ((s->tabCol + 1) == 13) {
            fprintf(stderr, "ERROR ON LINE %d: TOO MANY UNCOVERED CARDS ON COLUMN %d OF THE TABLEAU\n", s->line, s->tabs + 1);
            exit(-1);
        }
        g->tabUncovered[s->tabs][s->tabCol][0] = w[0];
        g->tabUncovered[s->tabs][s->tabCol++][1] = w[1];
    } else {
        if ((s->tabCol + 1) == 7) {
            fprintf(stderr, "ERROR ON LINE %d: TOO MANY COVERED CARDS ON COLUMN %d OF THE TABLEAU\n", s->line, s->tabs + 1);
            exit(-1);
        }
        g->tabCovered[s->tabs][s->tabCol][0] = w[0];
        g->tabCovered[s->tabs][s->tabCol++][1] = w[1];
    }
}

/**
 * Parses the stock section
 * @param g
 * the game to add parsed rules to
 * @param s
 * state of the parser
 * @param w
 * word that was most recently grabbed
 */
void parseStock(struct game *g, struct state *s, char w[]) {
    // Get the values of the card and suit
    int v = getCardValue(w[0]);
    enum SUITS c = getSuit(w[1]);

    // Check the type
    if (c == ERR && w[0] != '|') {
        fprintf(stderr, "ERROR ON LINE %d: %c IS AN INVALID CARD TYPE\n", s->line, w[1]);
        exit(-1);
    }

    // Check the value
    if (v <= 0 && w[0] != '|') {
        fprintf(stderr, "ERROR ON LINE %d: VALUE %c IS INVALID\n", s->line, w[0]);
        exit(-1);
    }

    // Watch for the breaking character
    if (w[0] == '|') {
        s->stock = 1;
        s->stockIndex = 0;
        return;
    }

    // Assign values to either the stock or waste
    if (s->stock) {
        if (s->stockIndex >= 24) {
            fprintf(stderr, "ERROR ON LINE %d: TOO MANY CARDS IN THE STOCK", s->line);
        }
        g->stock[s->stockIndex][0] = w[0];
        g->stock[s->stockIndex][1] = w[1];
        s->stockIndex++;
    } else {
        if (s->stockIndex >= 24) {
            fprintf(stderr, "ERROR ON LINE %d: TOO MANY CARDS IN THE WASTE", s->line);
        }
        g->waste[s->stockIndex][0] = w[0];
        g->waste[s->stockIndex][1] = w[1];
        s->stockIndex++;
    }
}

/**
 * Gets the current section we are in
 * @param w
 * word that was just read
 * @param s
 * pointer to the current state struct
 */
int checkSection(char w[], struct state *s, struct game *g) {
    int next = 0;
    switch (s->section) {
        // Checking for RULES section
        case -1:
            if (compare(w, SECTIONS[next])) {
                return next;
            } else {
                fprintf(stderr, "ERROR ON LINE %d: THE TEXT '%s' BEFORE RULES IS INVALID\n", s->line, w);
                exit(-1);
            }
            // Checking for FOUNDATIONS section
        case 0:
            next = 1;
            if (s->setTurn == -1 && s->setLimit == -1 && compare(w, SECTIONS[next])) {
                return next;
            } else if (s->setTurn != -1 && compare(w, SECTIONS[next])) {
                fprintf(stderr, "ERROR ON LINE %d: RULE 'turn' NOT SET\n", s->line);
                exit(-1);
            } else if (s->setLimit != -1 && compare(w, SECTIONS[next])) {
                fprintf(stderr, "ERROR ON LINE %d: RULE 'limit' NOT SET\n", s->line);
                exit(-1);
            } else {
                orderCheck(w, s, next);
                parseRules(g, s, w);
                return s->section;
            }
            // Checking for TABLEAU section
        case 1:
            next = 2;
            int b = 0;
            for (int i = 0; i < 4; b += s->setFoundations[i++]);
            if (b == 4 && compare(w, SECTIONS[next])) {
                return next;
            } else if (compare(w, SECTIONS[next])) {
                fprintf(stderr, "ERROR ON LINE %d: MISSING FOUNDATION. ONLY %d ARE SET\n", s->line, b);
                exit(-1);
            } else {
                orderCheck(w, s, next);
                parseFoundations(g, s, w);
                return s->section;
            }
            // Checking for STOCK section
        case 2:
            next = 3;
            if (s->tabs <= 0 && compare(w, SECTIONS[next])) {
                return next;
            } else if (compare(w, SECTIONS[next])) {
                fprintf(stderr, "ERROR ON LINE %d: MISSING TABLEAU COLUMN. ONLY %d ARE SET\n", s->line, 6 - s->tabs);
                exit(-1);
            } else {
                orderCheck(w, s, next);
                parseTableau(g, s, w);
                return s->section;
            }
            // Checking for MOVES section
        case 3:
            next = 4;
            if (compare(w, SECTIONS[next])) {
                return next;
            } else {
                orderCheck(w, s, next);
                parseStock(g, s, w);
                return s->section;
            }
        case 4:
            orderCheck(w, s, next);
            if (validateMove(w)) {
                addMove(g, w[0], w[3]);
            } else {
                fprintf(stderr, "ERROR ON LINE %d: MOVE '%s' IS INVALID\n", s->line, w);
            }
            return s->section;
        default:
            orderCheck(w, s, next);
            return s->section;
    }
}

struct game* inputParser(FILE *in) {
    struct game *g = getGame();
    struct state s = getState();
    int atTheBar = 0;
    for (char c = (char) fgetc(in); !feof(in); c = (char) fgetc(in)) {
        if (c == '\b') continue;
        if (c == '|') {
            if (atTheBar) {
                fprintf(stderr, "ERROR ON LINE %d: MULTIPLE '|' CHARACTERS IN A ROW IS INVALID", s.line);
                exit(-1);
            } else {
                atTheBar = 1;
            }
        } else {
            atTheBar = 0;
        }
        if (c == '#') {
            //Bleed off the rest of the line
            for (;c != '\n' && !feof(in); c = (char) fgetc(in));
        } else if (c != ' ' && c != '\n' && c != '\t') {
            char word[MAX_WORD_SIZE];
            ungetc(c, in);
            getWord(in, word, s);
            s.section = checkSection(word, &s, g);
        }

        // Its a new line. Increment the line counter
        if (c == '\n') {
            s.colChange = s.line;
            s.line++;
        }
    }

    // Bad file check
    if (s.section == -1) {
        fprintf(stderr, "GOT TO EOF WITHOUT ENCOUNTERING ANY SECTIONS\n");
        exit(-1);
    }

    validate(*g);
    return g;
}