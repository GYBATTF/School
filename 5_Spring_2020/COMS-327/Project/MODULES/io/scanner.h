#ifndef COMS_327_GROUP_PROJECT_SCANNER_H
#define COMS_327_GROUP_PROJECT_SCANNER_H

/**
 * Max size of a 'word' this number is based off the size of the word "FOUNDATIONS:"
 */
#define MAX_WORD_SIZE 13

/**
 * Sections to check for, this array is in the order that they need to appear in the document
 */
extern const char SECTIONS[5][MAX_WORD_SIZE];

/**
 * Parses the file input and adds information to a game struct.
 * Will stop the entire program and print an error if issues with the input file is found
 * @param in
 * input stream to read from, can either be stdin or a file pointer
 * @return
 * a game set up from the file
 */
struct game* inputParser(FILE *in);

#endif //COMS_327_GROUP_PROJECT_SCANNER_H
