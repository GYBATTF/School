/**
 * Handles running the main loop of the shell.
 * @author ajharms
 */
#include "processes.h"

/**
 * Runs the shell loop
 * @return
 * status code
 */
int main(void) {
    while(runShell());
    return 0;
}