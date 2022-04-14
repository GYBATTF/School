/**
 * Functions that are reused throughout the program.
 * @author ajharms
 */
#ifndef UTILS_H
#define UTILS_H

#include "bools.h"

/**
 * Program arguments are bad
 */
#define BAD_ARGS 1
/**
 * Input was not a number
 */
#define NOT_A_NUMBER 2
/**
 * Number input was <1
 */
#define NUMBER_TO_SMALL 3
/**
 * Failed to init semaphore
 */
#define SEMAPHORE_FAILURE 4
/**
 * Malloc failed
 */
#define MALLOC_ERROR 5
/**
 * Failed to init mutex
 */
#define MUTEX_FAILURE 6
/**
 * Failed to create condition variable
 */
#define CONDITION_FAILURE 7

/**
 * Prints an error to stderr. Does not stop the program.
 * @param err
 * error to print
 * @param emp
 * true to add '!' to end of the error
 */
void printErr(const char* err, boolean emp);

/**
 * Prints an error to stderr. Does stop the program.
 * @param err
 * error to print
 * @param emp
 * true to add '!' to end of the error
 * @param
 * exit code to exit with
 */
void printErrAndExit(const char* err, boolean emp, int exitCode);

#endif