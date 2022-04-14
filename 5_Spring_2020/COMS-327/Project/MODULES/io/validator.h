#ifndef COMS_327_GROUP_PROJECT_VALIDATOR_H
#define COMS_327_GROUP_PROJECT_VALIDATOR_H

#include "../game/game.h"

/**
 * Validates a game's card order on the tableau,
 * and the number of cards that there too many of
 * and too few of
 * @param g
 * game to validate
 */
void validate(struct game g);

#endif //COMS_327_GROUP_PROJECT_VALIDATOR_H
