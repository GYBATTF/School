/**
 * Header file for input
 * @author ajharms
 */
#ifndef INPUT_H
#define INPUT_H

/**
 * Operators to look for in the input
 */
typedef char Operator;
#define NO_OP 0x00
#define BACKGROUND_OP 0x01
#define IN_OP 0x02
#define OUT_OP 0x04

/**
 * Struct storing command typed in and
 * information needed to run it
 */
struct input {
    unsigned argc;
    char** argv;
    char* file;
    Operator op;
};

/**
 * Gets input and parses it.  Checks input for any errors.
 * @return
 * and input struct, if there are any errors in the input
 * the error will be printed and NULL is returned
 */
struct input* getInput();

/**
 * Properly frees an input struct
 * @param i
 * input struct to free
 */
void freeInput(struct input* i);

#endif
