/**
 * Program to ceasar encrypt a text file.
 * @author ajharms
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads.h"
#include "../util/errors.h"

/**
 * Size of input buffer
 */
#define MSG_SIZE 50
int getBufferSize();

/**
 * Exact number of args the program should have
 */
#define N_ARGS 2
char** getArgs();

/**
 * Gets the infile and outfile from the command line,
 * and prompts the user for buffer size.  After getting
 * input starts encrypting the file.
 * @param argc
 * number of args
 * @param argv
 * args
 * @return
 * 0 if successful
 */
int main(int argc, char** argv) {
    char** args = getArgs(argc, argv);

    encrypt(args[0], args[1], getBufferSize());

    return 0;
}

/**
 * Verifies program arguments and return them
 * (excluding arg for the program). Exits the
 * program if arguments are incorrect.
 * @param argc
 * number of args
 * @param argv
 * array of args
 * @return
 * the infile and outfile, with infile as index 1 and outfile as index 2
 */
char** getArgs(int argc, char** argv) {
    argc--;
    if (argc != N_ARGS) {
        char* fStr = "too %s options, correct usage is:\n>> encrypt352 <infile> <outfile>";
        char* amt = argc < N_ARGS ? "few" : "many";

        unsigned msgLen = strlen(fStr) + strlen(amt) + 1;
        char msg[msgLen];

        sprintf(msg, fStr, amt);
        printErrAndExit(msg, false, BAD_ARGS);
    }
    return argv+1;
}

/**
 * Gets the size of the buffer from the user,
 * exits the program if the user doesn't enter
 * a number.
 * @return
 * size of buffer to use
 */
int getBufferSize() {
    char msg[MSG_SIZE];

    int digits = 0;
    for (int ms = MSG_SIZE-1; ms > 0; digits++, ms /= 10);

    char sizedFormat[digits + 3];
    sprintf(sizedFormat, "%%%ds", MSG_SIZE-1);

    printf("Enter buffer size: ");
    scanf(sizedFormat, msg);
    printf("\n");
    
    char* end;
    int bufferSize = strtol(msg, &end, 10);

    char* fStr;
    int err = 0;
    if (end == msg || *end != '\0') {
        fStr = "'%s' is not a number";
        err = NOT_A_NUMBER;
    } else if (bufferSize < 1) {
        fStr = "'%s' must be greater than zero";
        err = NUMBER_TO_SMALL;
    }

    if (err) {
        char msg2[strlen(fStr)+MSG_SIZE+1];
        sprintf(msg2, fStr, msg);
        printErrAndExit(msg2, true, err);
    }
    return bufferSize;
}
