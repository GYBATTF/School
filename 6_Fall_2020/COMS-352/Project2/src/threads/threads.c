/**
 * This file manages starting and waiting for the threads.
 *
 * Threads run in a loop popping (or reading) characters,
 * performing the necessary operation on them, and then
 * pushing (or writing) the character.
 *
 * When EOF is popped/read it is pushed/written and then
 * the thread stops. When all threads complete the finishing
 * message is printed.
 *
 * @author ajharms
 */
#include "threads.h"

#include <stdio.h>
#include <pthread.h>

#include "../supplied/encrypt.h"
#include "../util/buffer.h"
#include "../util/print_counts.h"
#include "../util/bools.h"

/**
 * Reads a file character by character, pushing it to the IN_COUNTER buffer.
 * If the character is EOF, it pushes it then exits.
 * @param infile
 * file to read from
 * @return
 * nothing
 */
void* reader(void* infile) {
    open_input((char*) infile);
    while (true) {
        pop(READER);
        int c = read_input();
        push(READER, c);
        if (c == EOF) return NULL;
    }
}

/**
 * Counts a character that has been read and passes it to the next thread.
 * If the character is EOF pushes it and then exits.
 * @return
 * nothing
 */
void* incounter() {
    while (true) {
        int c = pop(IN_COUNTER);
        if (c != EOF) count_input(c);
        push(IN_COUNTER, c);
        if (c == EOF) return NULL;
    }
}

/**
 * Encrypts a character that has been counted.
 * If the character is EOF pushes it and then exits.
 * @return
 * nothing
 */
void* encryptor() {
    while (true) {
        int c = pop(ENCRYPTOR);
        if (c != EOF) c = caesar_encrypt(c);
        push(ENCRYPTOR, c);
        if (c == EOF) return NULL;
    }
}

/**
 * Counts a character that has been encrypted.
 * If the character is EOF pushes it then exits.
 * @return
 * nothing
 */
void* outcounter() {
    while (true) {
        int c = pop(OUT_COUNTER);
        if (c != EOF) count_output(c);
        push(OUT_COUNTER, c);
        if (c == EOF) return NULL;;
    }
}

/**
 * Pops a character from its buffer, and writes it to file.
 * If the character is EOF, quits.
 * @param outfile
 * file to output to
 * @return
 * nothing
 */
void* writer(void* outfile) {
    open_output((char*) outfile);
    while(true) {
        int c = pop(WRITER);
        if (c == EOF) return NULL;
        write_output(c);
        push(WRITER, 0);
    }
}

/**
 * Thread pids to watch for completion
 */
pthread_t THREAD_IDS[NUM_THREADS];

/**
 * Constant array containing the threads to start, in the same order as the THREAD enum
 */
typedef void* (*thread)(void*);
const thread THREADS[NUM_THREADS] = {reader, incounter, encryptor, outcounter, writer};

/**
 * Caesar encrypts infile to outfile using multiple threads.
 * @param infile
 * file to read from
 * @param outfile
 * file to write to
 * @param bufferSize
 * size of buffer to use while encrypting
 */
void encrypt(char* infile, char* outfile, unsigned bufferSize) {
    buffer_init(bufferSize);

    for (int i = 0; i < NUM_THREADS; i++) {
        char* arg = NULL;
        arg = i == READER ? infile : arg;
        arg = i == WRITER ? outfile : arg;
        pthread_create(&THREAD_IDS[i], NULL, THREADS[i], arg);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(THREAD_IDS[i], NULL);

    printCounts();
    printf("End of file reached\n\n");
}

/**
 * Returns the string representation of the enum value
 * @param t
 * enum to convert to string
 * @return
 * string name of the enum value
 */
char* thread2string(enum THREADS t) {
    switch (t) {
        case READER: return "READER";
        case IN_COUNTER: return "IN_COUNTER";
        case ENCRYPTOR: return "ENCRYPTOR";
        case OUT_COUNTER: return "OUT_COUNTER";
        case WRITER: return "WRITER";
        default: return NULL;
    }
}