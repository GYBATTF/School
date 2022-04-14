#include <termbox.h>
#include <iostream>
#include "interface.hh"
#include <termbox.h>

#define HEART 9829
#define CLUB 9827
#define SPADE 9824
#define DIAMOND 9830

interface::interface() {
    in = NULL;
    seedling = 0;
    turnOne = true;
    flips = -1;
    started = false;
    boxmod = false;
    from = '\0';
    h = new struct history;
    h->previous = NULL;
}

interface::~interface() {
    if (started) tb_shutdown();

    while (h->previous != NULL) {
        struct history *nextVictim = h->previous;
        delete(h);
        h = nextVictim;
    }
    delete(h);
    
    std::cout << "Done!\n";
}

void interface::turn1() {
    turnOne = true;
}

void interface::turn3() {
    turnOne = false;
}

void interface::limit(unsigned limit) {
    flips = limit;
}

void interface::seed(unsigned s) {
    seedling = s;
}

void interface::file(char *file) {
    filename = file;
    if (in != NULL) fclose(in);
    in = fopen(file, "r");
    // Strip path out to just filename
    while (char * f = strchr(filename, '/')) filename = f + 1;
}

void interface::start() {
    // Read file or generate game
    struct game *g;
    if (in != NULL) {
        g = inputParser(in);
        fclose(in);
        turnOne = h->g.turn == 1;
    } else {
        g = getRandomGame(seedling, flips, turnOne ? 1 : 3);
    }
    memcpy(&h->g, g, sizeof(struct game));
    freeGame(g);
    h->g.output = NULL;
    h->g.moves = NULL;
    
    // Initialize display
    tb_init();
    started = true;
    init();

    // Begin input loop
    for (;;) {
        // Reminding user where they are moving a card from
        if (boxmod) {
            boxmod = false;
            from = '\0';
            struct tb_cell drawline;
            drawline.bg = TB_GREEN;
            drawline.fg = TB_GREEN;
            drawline.ch = '\0';
            for (int i = 0; i < 34; i++) {
                tb_put_cell(i, 7, &drawline);
            }
        } else if (from != '\0') {
            boxmod = true;
            char ft[40];
            char f = from;
            if (f == 'w') f = 'W';
            if (f == 'f') f = 'F';
            sprintf(ft, "Moving from: %c", f);
            struct tb_cell fr;
            fr.bg = TB_GREEN;
            fr.fg = TB_BLACK;
            char *ftp = ft;
            for (int i = 10; *ftp != '\0'; ftp++, i++) {
                fr.ch = *ftp;
                tb_put_cell(i, 7, &fr);
            }
        }

        // Draw everything
        drawFoundations();
        drawTableau();
        drawWaste();
        tb_present();

        // Parse input
        struct tb_event event;
        tb_poll_event(&event);

        switch (event.ch) {
            case 'q': case 'Q':
                // Bail out
                return;
            case 'u': case 'U':
                undo();
                continue;
            case '.':
                move('.', '\0');
                continue;
            case '>':
                move('r', '\0');
                continue;
            case '1': case '2': case '3': case '4': case '5': case '6':
            case '7': case 'w': case 'W': case 'f': case 'F':
                if (from == 'F') from = 'f';
                if (from == 'W') from = 'w';
                if (from) {
                    move(from, event.ch);
                } else {
                    from = event.ch;
                }
        }
    }
}

void interface::init() {
    struct tb_cell cell;
    cell.ch = '\0';
    cell.bg = TB_GREEN;
    cell.fg = TB_GREEN;

    // Draw the box
    for (int i = 0; i < 34; i++) {
        tb_put_cell(i, 0, &cell);
        tb_put_cell(i, 7, &cell);
    }
    for (int i = 1; i < 7; i++) {
        tb_put_cell(0, i, &cell);
        tb_put_cell(33, i, &cell);
    }

    // Add how to move text
    const char* text1 = "To move, press the source and\nthen destination (one of W, F,\n     1, 2, 3, 4, 5, 6, 7)";
    cell.bg = TB_BLACK;
    cell.fg = TB_WHITE;
    for (int r = 4, c = 2; *text1 != '\0'; text1++, c++) {
        if (*text1 == '\n') {
            r++;
            c = 1;
        } else {
            cell.ch = *text1;
            tb_put_cell(c, r, &cell);
        }
    }

    // Display hot keys
    const char* text2 = "Q: quit         U: undo move\n\n.: next card    >: reset stock";
    for (int r = 1, c = 2; *text2 != '\0'; text2++, c++) {
        if (*text2 == '\n') {
            r++;
            c = 1;
        } else {
            if (*(text2 + 1) == ':') {
                cell.bg = TB_MAGENTA;
                cell.fg = TB_BLACK;
            } else {
                cell.bg = TB_BLACK;
                cell.fg = TB_WHITE;
            }
            cell.ch = *text2;
            tb_put_cell(c, r, &cell);
        }
    }

    // Tableau labels
    cell.bg = TB_MAGENTA;
    cell.fg = TB_BLACK;
    for (int t = 1, c = 39; t < 8; t++, c += 6) {
        cell.ch = '0' + t;
        tb_put_cell(c, 0, &cell);
    }

    // Foundations label
    const char* foundations = "Foundations";
    cell.bg = TB_MAGENTA;
    cell.fg = TB_BLACK;
    for (int c = 11; *foundations != '\0'; foundations++, c++) {
        cell.ch = *foundations;
        tb_put_cell(c, 9, &cell);
        cell.bg = TB_BLACK;
        cell.fg = TB_WHITE;
    }

    // Waste label, including how many flips are left
    char w[40];
    if (h->g.limit == -1) sprintf(w, "Waste");
    else sprintf(w, "Waste  (  resets remaining)");
    cell.bg = TB_MAGENTA;
    cell.fg = TB_BLACK;
    char *waste = w;
    for (int c = h->g.limit == -1 ? 14 : 3; *waste != '\0'; waste++, c++) {
        cell.ch = *waste;
        tb_put_cell(c, 16, &cell);
        cell.bg = TB_BLACK;
        cell.fg = TB_WHITE;
    }

    // Display seed/filename as centered as possible
    char t[34];
    char *title = t;
    if (in == NULL) {
        sprintf(t, "Seed: %ld", seedling);
    } else {
        sprintf(t, "File: %s", filename);
    }
    int size = strlen(t);
    size = 34 - size;
    size /= 2;
    cell.bg = TB_GREEN;
    cell.fg = TB_BLACK;
    for (; *title != '\0'; title++, size++) {
        cell.ch = *title;
        tb_put_cell(size, 0, &cell);
    }
}

void interface::drawFoundations() {
    struct tb_cell cell;
    cell.bg = TB_GREEN;
    cell.fg = TB_BLACK;
    // Just draw all the backs then cover them up 
    for (int r = 11; r < 15; r++) {
        int c = 4;
        cell.ch = CLUB;
        for (; c < 8; c++) tb_put_cell(c, r, &cell);
        c += 4;
        cell.ch = DIAMOND;
        for (; c < 16; c++) tb_put_cell(c, r, &cell);
        c += 4;
        cell.ch = HEART;
        for (; c < 24; c++) tb_put_cell(c, r, &cell);
        c += 4;
        cell.ch = SPADE;
        for (; c < 32; c++) tb_put_cell(c, r, &cell);
    }

    // Draw the top of the foundations
    char card[2];
    if (h->g.foundations[0] > 0) {
        card[0] = getCardValueChar(h->g.foundations[0]);
        card[1] = 'c';
        drawCard(4, 11, card);
    }

    if (h->g.foundations[1] > 0) {
        card[0] = getCardValueChar(h->g.foundations[1]);
        card[1] = 'd';
        drawCard(12, 11, card);
    }

    if (h->g.foundations[2] > 0) {
        card[0] = getCardValueChar(h->g.foundations[2]);
        card[1] = 'h';
        drawCard(20, 11, card);
    }

    if (h->g.foundations[3] > 0) {
        card[0] = getCardValueChar(h->g.foundations[3]);
        card[1] = 's';
        drawCard(28, 11, card);
    }
}

void interface::drawWaste() {
    struct tb_cell cell;
    // Get the sizes of them
    int wSize = sizeOfPile(*h->g.waste, MAX_WASTE);
    int sSize = sizeOfPile(*h->g.stock, MAX_STOCK);

    // If theres cards draw the back else draw empty space
    if (sSize) {
        drawBack(25, 18);
    } else {
        drawBlank(25, 18);
    }

    // Clear out the space where the cards are drawn so we can redraw them
    cell.bg = TB_BLACK;
    cell.fg = TB_BLACK;
    cell.ch = '\0';
    for (int r = 18; r < 23; r++) {
        for (int c = 7; c < 15; c++) {
            tb_put_cell(c, r, &cell);
        }
    }

    // Draw the cards
    if (wSize > 0) {
        if (turnOne) {
            drawCard(7, 18, h->g.waste[wSize - 1]);
            wSize = 1;
        } else {
            int first = wSize - 3;
            if (first < 0) first = 0;
            else wSize = 3;
            if (h->g.waste[first][0] != '\0') drawCard(7, 18, h->g.waste[first++]);
            if (h->g.waste[first][0] != '\0') drawCard(9, 18, h->g.waste[first++]);
            if (h->g.waste[first][0] != '\0') drawCard(11, 18, h->g.waste[first]);
        }
    } else {
        drawBlank(7, 18);
    }

    // Draw the number of cards in each stack
    cell.bg = TB_BLACK;
    cell.fg = TB_WHITE;
    char num[3];
    int nums[] = {wSize, sSize};
    int numx[] = {8, 26};
    for (int i = 0; i < 2; i++) {
        sprintf(num, "%d", nums[i]);
        char *n = num;
        for (int c = numx[i]; *n != '\0'; n++, c++) {
            cell.ch = *n;
            tb_put_cell(c, 22, &cell);
        }
    }

    // Draw the number of resets left if neccesary
    if (h->g.limit != -1) {
        cell.bg = TB_BLACK;
        cell.fg = TB_WHITE;
        cell.ch = '0' + h->g.limit;
        tb_put_cell(11, 16, &cell);
    }
}

void interface::drawTableau() {
    struct tb_cell cell;
    // Clear the space to redraw
    cell.bg = TB_BLACK;
    cell.fg = TB_BLACK;
    cell.ch = '\0';
    for (int r = 1; r < tb_height(); r++) {
        for (int c = 34; c < tb_width(); c++) {
            tb_put_cell(c, r, &cell);
        }
    }

    // Draw the columns
    for (int t = 0, c = 38; t < 7; t++, c += 6) {
        int r = 1;
        int cSize = sizeOfPile(*h->g.tabCovered[t], MAX_COVERED);
        int uSize = sizeOfPile(*h->g.tabUncovered[t], MAX_UNCOVERED);
        for (int i = 0; i < cSize; i++, r++) drawBack(c, r);
        for (int i = 0; i < uSize; i++, r++) drawCard(c, r, h->g.tabUncovered[t][i]);
    }
}

void interface::drawCard(unsigned x, unsigned y, char* card) {
    struct tb_cell cell;
    cell.ch = '\0';
    cell.bg = TB_WHITE;

    // White out the area
    for (int r = y; r - y < 4; r++) {
        for (int c = x; c - x  < 4; c++) {
            tb_put_cell(c, r, &cell);
        }
    }

    // Set the suit character and color
    switch (card[1]) {
        case 'c':
            cell.ch = CLUB;
            cell.fg = TB_BLACK;
            break;
        case 's':
            cell.ch = SPADE;
            cell.fg = TB_BLACK;
            break;
        case 'h':
            cell.ch = HEART;
            cell.fg = TB_RED;
            break;
        case 'd':
            cell.ch = DIAMOND;
            cell.fg = TB_RED;
    }

    // Draw the suit
    tb_put_cell(x + 1, y, &cell);
    tb_put_cell(x + 2, y + 3, &cell);

    // Draw the rank
    cell.ch = card[0];
    tb_put_cell(x, y, &cell);
    tb_put_cell(x + 3, y + 3, &cell);
}

void interface::drawBack(unsigned x, unsigned y) {
    struct tb_cell cell;
    // Draw the back of a card
    cell.bg = TB_CYAN;
    cell.fg = TB_BLACK;
    cell.ch = '#';
    for (int r = y; r < y + 4; r++) {
        for (int c = x; c < x + 4; c++) {
            tb_put_cell(c, r, &cell);
        }
    }
}

void interface::drawBlank(unsigned x, unsigned y) {
    struct tb_cell cell;
    // Clear the spot out
    cell.bg = TB_BLACK;
    cell.fg = TB_BOLD | TB_RED;
    cell.ch = '\0';
    for (int r = y; r < y + 4; r++) {
        for (int c = x; c < x + 4; c++) {
            tb_put_cell(c, r, &cell);
        }
    }
    // Draw an X
    cell.ch = 9586;
    tb_put_cell(x, y, &cell);
    tb_put_cell(x + 1, y + 1, &cell);
    tb_put_cell(x + 2, y + 2, &cell);
    tb_put_cell(x + 3, y + 3, &cell);
    cell.ch = 9585;
    tb_put_cell(x + 3, y, &cell);
    tb_put_cell(x + 2, y + 1, &cell);
    tb_put_cell(x + 1, y + 2, &cell);
    tb_put_cell(x, y + 3, &cell);
}

void interface::undo() {
    if (h->previous == NULL) return;
    h = h->previous;
}

void interface::move(char from, char to) {
    if (from == 'f' || to == 'w') return;
    struct history *todo = new struct history;
    todo->previous = h;
    h = todo;
    memcpy(&h->g, &h->previous->g, sizeof(struct game));
    addMove(&h->g, from, to);
    int succ = playMove(&h->g, 0);
    if (!succ) undo();
}