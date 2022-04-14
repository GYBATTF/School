#ifndef COMS_327_GROUP_PROJECT_SOLVE_HH
#define COMS_327_GROUP_PROJECT_SOLVE_HH

extern "C" {
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include "moves.h"
    #include "../io/scanner.h"
    #include "../io/output.h"
    #include "../game/game.h"
    #include "../game/card.h"
}

void solve(struct game *g);

class solver {
    private:
        // Use a cache to find solution
        bool useCache;
        // Move limit
        unsigned mlimit;
        // verbose
        bool v;
        bool a;
        // force foundations
        bool forced;

        // Linked list of moves
        struct move *moves;
        int moveIndex;
        // Initial state of the game
        struct game *initial;
        // Number of attempts made to find a solution
        long long attempts;

    public:
        // Constructor
        solver();

        // Set solver conditions
        void verbose();
        void all();
        void forcedFoundations();
        void cache();
        void moveLimit(unsigned limit);

        // Get the game and solve it
        void solve(FILE* input);

    private:
        // Not implemented
        void cacheSolver();

        // Recursive function to find a solution
        void findSolution(struct game *g, unsigned timer);
        // Function to force fonudations first when finding the solutions
        void findForcedFoundations(struct game *g, unsigned timer);
        // Makes a move and sees if it works, if it does calls findsolution
        void attempt(struct game *g, unsigned timer, char src, char dest);
        // Test to see if the game is wonm if it is prints moves and exits
        void checkwin(struct game *g, unsigned timer);
};

#endif //COMS_327_GROUP_PROJECT_SOLVE_HH