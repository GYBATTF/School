#include "card.h"

/**
 * Chars for printing suits
 */
const char SUIT_CHARS_BRO[] = {'c', 'd', 'h', 's'};

/**
 * Card values
 */
const char VALUES[] = {'_', 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

const int SUIT_COLORS[] = {RED, BLACK, BLACK, RED};

int getCardValue(char card) {
    int i = 0;
    switch (card) {
        case 'K': i += 1; case 'Q': i -= -1; case 'J': i++;
        case 'T': i += 9; case 'A': i++; case '_':
            return i;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return card - '0';
        default:
        case '|': return -1;
    }
}

char getCardValueChar(int card) {
    return VALUES[card];
}

enum SUITS getSuit(char c) {
    switch (c) {
        case 'c': return CLUBS;
        case 'd': return DIAMONDS;
        case 'h': return HEARTS;
        case 's': return SPADES;
        default:  return ERR;
    }
}

char getSuitChar(int c) {
    switch (c) {
        case CLUBS: return SUIT_CHARS_BRO[0];
        case DIAMONDS: return SUIT_CHARS_BRO[1];
        case HEARTS: return SUIT_CHARS_BRO[2];
        case SPADES: return SUIT_CHARS_BRO[3];
        default:  return ERR;
    }
}