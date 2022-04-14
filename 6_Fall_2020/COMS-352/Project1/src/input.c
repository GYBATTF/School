/**
 * This file handles getting input from the shell, parsing it, and checking if it is valid.
 * After getting the input it cleans it up, argizes it, and checks for the <, >, and & operators.
 * If file redirection is found the file is attempted to be opened.
 * @author ajharms
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bools.h"
#include "input.h"

/**
 * A single arg of input.
 * These are stored as a linked list.
 * If it is an operator the operator type is stored.
 */
struct arg {
    char* t;
    unsigned len;
    struct arg* next;
    Operator op;
};

/**
 * Frees a arg list from memory.
 * @param t
 * arg list to free
 */
unsigned freeArgList(struct arg* t) {
    unsigned rm = 0;
    while (t != NULL) {
        rm++;
        struct arg* old = t;
        char* s = t->t;
        struct arg* next = t->next;
        free(s);
        free(old);
        t = next;
    }
    return rm;
}

/**
 * Removes chars from input that unnecessary
 * @param line
 * line of input
 * @param size
 * size of line
 * @return
 * new line free of bad char, NULL if malloc failed
 */
char* removeBadChars(const char* line, unsigned size) {
    char* newLine = calloc(size + 1, sizeof(char));
    if (newLine == NULL) {
        printf("shell352: Failed to read input, malloc error!\n");
        return NULL;
    }

    boolean atChars = false;
    int j = 0;
    for (int i = 0; i < size && line[i] != '\0'; i++) {
        switch (line[i]) {
        case '\b': case '\t': case '\n':
            continue;
        case ' ':
            if (!atChars) continue;
        default:
            atChars = true;
            newLine[j] = line[i];
            j++;
        }
    }

    newLine[j] = '\0';
    for (j += 1; j >= 0; j--) {
        boolean done = false;
        switch (newLine[j]) {
        case '\b': case '\t': case '\n': case ' ':
            newLine[j] = '\0';
            break;
        default:
            done = true;
        }
        if (done) break;
    }
    
    return newLine;
}

/**
 * Reads a line into the shell from stdin
 * @return
 * a list of args from the input
 */
struct arg* readLine() {
    struct arg* first = malloc(sizeof(struct arg));
    first->len = 0;
    first->next = NULL;
    first->t = NULL;
    first->op = NO_OP;

    // Read input, returning early if nothing was entered
    size_t len = 0;
    char* in = NULL;
    int read = getline(&in, &len, stdin);
    if (read == -1) {
        free(first);
        printf("shell352: Failed to read input!\n");
        return NULL;
    } else if (read == 0) {
        free(first);
        return NULL;
    }

    char* line = removeBadChars(in, read);
    free(in);
    if (line == NULL) {
        free(first);
        return NULL;
    }

    // Break the line into each arg
    char* arg = strtok(line, " ");
    struct arg* current = first;
    while (arg != NULL) {
        // Create next link
        current->next = calloc(1, sizeof(struct arg));
        current = current->next;
        if (current == NULL) {
            freeArgList(first);
            free(line);
            printf("shell352: Failed to parse input, malloc error!\n");
            return NULL;
        }

        // Initialize link string
        current->next = NULL;
        current->len = strlen(arg);
        current->t = malloc(sizeof(char*) * (current->len + 1));
        if (current->t == NULL) {
            freeArgList(first);
            free(line);
            printf("shell352: Failed to parse input, malloc error!\n");
            return NULL;
        }

        // Add values to link
        strcpy(current->t, arg);
        if (current->len == 1) {
            switch (current->t[0]) {
                case '&':
                    current->op = BACKGROUND_OP;
                    break;
                case '<':
                    current->op = IN_OP;
                    break;
                case '>':
                    current->op = OUT_OP;
            }
        }

        // Get next arg
        first->len++;
        arg = strtok(NULL, " ");
    }

    // Free line recieved from getline()
    free(line);
    return first;
}

/**
 * Determines if the argv is to be run in the background
 * and adds the result to an input struct. If the background
 * operator is found, it is removed from the list of args.
 * Should be the first function run against the arg list.
 * @param i
 * Pointer to input struct
 * @param t
 * Pointer to arg struct
 */
void isBackground(struct input* i, struct arg* t) {
    if (i == NULL || t == NULL) return;
    struct arg* head = t;
    while (t != NULL) {
        if (t->next != NULL) {
            if (t->next->op == BACKGROUND_OP) {
                if (t->next->next == NULL) {
                    freeArgList(t->next);
                    t->next = NULL;

                    head->len--;
                    i->op |= BACKGROUND_OP;
                    return;
                }
            }
        }
        t = t->next;
    }
}

/**
 * Determines if there's a redirect to/from file in the argv
 * and if there is opens the file and adds it to the input struct.
 * This should the the second functions to run against the arg list.
 * @param i
 * Pointer to input struct
 * @param current
 * Pointer to arg struct
 * @return
 * true if no syntax error in argv, false if there is a syntax error.
 * A syntax error is if the argv contains multiple redirects, no file
 * is specified after the redirect, or there are more than one args after
 * the redirect operator.
 */
boolean hasRedirect(struct input* i, struct arg* t) {
    if (i == NULL || t == NULL) return false;

    struct arg* previous = NULL;
    struct arg* current = t;
    while (current != NULL) {
        if (current->op == IN_OP || current->op == OUT_OP) {
            // No command to redirect to
            if (previous == NULL) return false;
            // How did this happen?
            if (current->t == NULL) return false;

            // No file to read/write
            if (current->next == NULL) {
                printf("syntax error\n");
                return false;
            }


            // Check to make sure nothing else is past the file specified
            char* file = current->next->t;
            if (current->next->next != NULL) {
                printf("shell352: Redirect file \"%s\" already specified!\n", file);
                return false;
            }
            i->file = malloc(sizeof(char) * (strlen(file) + 1));
            if (i->file == NULL) {
                printf("shell352: Failed to parse input, malloc error!\n");
                return false;
            }
            strcpy(i->file, file);
            i->op |= current->op;

            // Trim arg list
            t->len -= freeArgList(current);
            previous->next = NULL;

            return true;
        }
        previous = current;
        current = current->next;
    }
    return true;
}

/**
 * Turns the remaining args in the list into an array of char
 * arrays and adds it to an input struct.
 * This should be the last function to run against the arg list.
 * @param i
 * Pointer to input struct
 * @param t
 * Pointer to arg struct
 * @return
 * true if successful, false if not successful or if there are no args
 */
boolean argsToStringArray(struct input* i, struct arg* t) {
    if (i == NULL || t == NULL || t->next == NULL) return false;

    // Create array to hold args
    char** args = calloc(t->len + 1, sizeof(char**));
    if (args == NULL) {
        printf("shell352: Failed to parse input, malloc error!\n");
        return false;
    }

    t = t->next;
    int j = 0;
    while (t != NULL) {
        // Create array to hold arg
        args[j] = calloc(t->len, sizeof(char*));
        if (args[j] == NULL) {
            printf("shell352: Failed to parse input, malloc error!\n");
            for (int k = 0; k < j; k++) free(args[k]);
            free(args);
            return false;
        }

        // Copy arg into array
        strcpy(args[j], t->t);
        j++;
        t = t->next;
    }

    args[j] = NULL;
    i->argv = args;
    i->argc = j;
    return true;
}

/**
 * Creates an input struct and initializes it to the proper values
 * @return 
 * an input struct, or NULL if malloc fails
 */
struct input* allocInput() {
    struct input* i = malloc(sizeof(struct input));
    if (i == NULL) {
        printf("shell352: Failed to read input, malloc error!\n");
        return NULL;
    }
    i->argc = 0;
    i->argv = NULL;
    i->file = NULL;
    i->op = NO_OP;
    return i;
}

/**
 * Gets input and parses it.  Checks input for any errors.
 * @return 
 * and input struct, if there are any errors in the input
 * the error will be printed and NULL is returned
 */
struct input* getInput() {
    // Get input struct
    struct input* in = allocInput();
    if (in == NULL) return NULL;

    // Get line from stdin
    struct arg* line = readLine();
    
    // Check to see if we need to run in the background
    isBackground(in, line);
    
    // Check for redirects
    if (!hasRedirect(in, line)) {
        freeArgList(line);
        freeInput(in);
        return NULL;
    }
    
    // Convert the rest of the args into something that execvp can use
    if (!argsToStringArray(in, line)) {
        freeArgList(line);
        freeInput(in);
        return NULL;
    }
    
    // Free up the arg list
    freeArgList(line);

    // If there's no args, there's nothing to do anyway
    if (in->argc == 0) {
        freeInput(in);
        return NULL;
    }

    // Successfully finished parsing
    return in;
}

/**
 * Properly frees an input struct
 * @param i 
 * input struct to free
 */
void freeInput(struct input* i) {
    if (i == NULL) return;
    for (int j = 0; j < i->argc; j++) free(i->argv[j]);
    free(i->argv);
    free(i->file);
    free(i);
}