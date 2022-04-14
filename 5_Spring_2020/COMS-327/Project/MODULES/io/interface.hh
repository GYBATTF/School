#ifndef COMS_327_GROUP_PROJECT_INTERFACE_HH
#define COMS_327_GROUP_PROJECT_INTERFACE_HH

extern "C" {
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include "../moves/moves.h"
    #include "scanner.h"
    #include "output.h"
    #include "../game/game.h"
    #include "../game/card.h"
}

struct history {
    struct game g;
    struct history *previous;
};

class interface {
    bool started;
    bool turnOne;
    int flips;
    unsigned long seedling;
    FILE* in;
    char* filename;

    bool boxmod;
    char from;
    
    struct history *h;

    public:
        interface();
        ~interface();
        void turn1();
        void turn3();
        void limit(unsigned l);
        void seed(unsigned s);
        void file(char *file);
        void start();

    private:
        void init();
        void undo();
        void drawFoundations();
        void drawWaste();
        void drawTableau();
        void drawCard(unsigned x, unsigned y, char* card);
        void drawBack(unsigned x, unsigned y);
        void drawBlank(unsigned x, unsigned y);
        void move(char from, char to);
};

#endif //COMS_327_GROUP_PROJECT_INTERFACE_HH