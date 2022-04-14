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
#ifndef THREADS_H
#define THREADS_H

enum THREADS{READER, IN_COUNTER, ENCRYPTOR, OUT_COUNTER, WRITER, NUM_THREADS};

/**
 * Caesar encrypts infile to outfile using multiple threads.
 * @param infile
 * file to read from
 * @param outfile
 * file to write to
 * @param bufferSize
 * size of buffer to use while encrypting
 */
void encrypt(char* infile, char* outfile, unsigned bufferSize);

/**
 * Returns the string representation of the enum value
 * @param t
 * enum to convert to string
 * @return
 * string name of the enum value
 */
char* thread2string(enum THREADS t);

#endif
