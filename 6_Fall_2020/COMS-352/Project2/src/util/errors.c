/**
 * Functions that are reused throughout the program.
 * @author ajharms
 */
#include "errors.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Prints an error to stderr. Does not stop the program.
 * @param err
 * error to print
 * @param emp
 * true to add '!' to end of the error
 */
void printErr(const char* err, boolean emp) {
    fprintf(stderr, "Error, %s%s\n", err, emp ? "!" : "");
}

/**
 * Prints an error to stderr. Does stop the program.
 * @param err
 * error to print
 * @param emp
 * true to add '!' to end of the error
 * @param
 * exit code to exit with
 */
void printErrAndExit(const char* err, boolean emp, int exitCode) {
    printErr(err, emp);
    exit(exitCode);
}