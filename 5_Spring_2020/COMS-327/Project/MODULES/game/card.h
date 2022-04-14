#ifndef COMS_327_GROUP_PROJECT_CARD_H
#define COMS_327_GROUP_PROJECT_CARD_H

/**
 * ENUM to track the possible suits of cards and index arrays
 */
enum SUITS {CLUBS, DIAMONDS, HEARTS, SPADES, ERR};

/**
 * Suit colors: red = 0, black = 1
 */
enum COLORS {RED, BLACK};

/**
 * Maps colors to suits
 */
extern const int SUIT_COLORS[];

/**
 * Converts a card's char to a value
 * @param card
 * char card value
 * @return
 * card value as an int
 */
int getCardValue(char card);

/**
 * Get vhar for the card value
 * @param card
 * card to get char for
 * @return
 * char for that card
 */
char getCardValueChar(int card);

/**
 * Converts a char into an index
 * @param c
 * char to check
 * @return
 * index of suit
 */
enum SUITS getSuit(char c);

/**
 * Converts numberical card value to character
 * @param c
 * value to convert
 * @return
 * character for that value
 */
char getSuitChar(int c);

#endif //COMS_327_GROUP_PROJECT_CARD_H
