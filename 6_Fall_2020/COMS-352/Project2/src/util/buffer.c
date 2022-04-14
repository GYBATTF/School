/**
 * Buffer to process characters in a text file and encrypt them
 * @author ajharms
 */
#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#include "../supplied/encrypt.h"
#include "errors.h"
#include "print_counts.h"

/**
 * Deallocates the currently allocated buffer
 */
void freeBuffer();

/**
 * Buffer to move data across threads
 */
typedef struct {
    unsigned capacity;

    struct Queue {
        sem_t size;
        pthread_mutex_t lock;
        unsigned s;

        boolean shouldSignal;
        pthread_cond_t signal;

        struct Node {
            int c;
            struct Node* next;
        } *head;
        struct Node* tail;
    } *buffers[NUM_THREADS];
} Buffer;

/**
 * Global buffer
 */
Buffer buffer;

/**
 * Locks down the threads so that the encryptor can properly reset
 */
void reset_requested() {
    pthread_mutex_lock(&buffer.buffers[READER]->lock);

    int semVal;
    sem_getvalue(&buffer.buffers[IN_COUNTER]->size, &semVal);
    if (semVal > 0) {
        pthread_mutex_lock(&buffer.buffers[IN_COUNTER]->lock);
        buffer.buffers[IN_COUNTER]->shouldSignal = true;
        pthread_cond_wait(&buffer.buffers[IN_COUNTER]->signal, &buffer.buffers[IN_COUNTER]->lock);
    }

    sem_getvalue(&buffer.buffers[ENCRYPTOR]->size, &semVal);
    if (semVal > 0) {
        pthread_mutex_lock(&buffer.buffers[ENCRYPTOR]->lock);
        buffer.buffers[ENCRYPTOR]->shouldSignal = true;
        pthread_cond_wait(&buffer.buffers[ENCRYPTOR]->signal, &buffer.buffers[ENCRYPTOR]->lock);
    }

    sem_getvalue(&buffer.buffers[OUT_COUNTER]->size, &semVal);
    if (semVal > 0) {
        pthread_mutex_lock(&buffer.buffers[OUT_COUNTER]->lock);
        buffer.buffers[OUT_COUNTER]->shouldSignal = true;
        pthread_cond_wait(&buffer.buffers[OUT_COUNTER]->signal, &buffer.buffers[OUT_COUNTER]->lock);
    }

    printCounts();
}

/**
 * Unlocks the encryptor so that it can continue after a reset
 */
void reset_finished() {
    printf("Reset finished\n\n");

    pthread_mutex_unlock(&buffer.buffers[READER]->lock);
    if (buffer.buffers[IN_COUNTER]->shouldSignal) {
        buffer.buffers[IN_COUNTER]->shouldSignal = false;
        pthread_mutex_unlock(&buffer.buffers[IN_COUNTER]->lock);
    }
    if (buffer.buffers[ENCRYPTOR]->shouldSignal) {
        buffer.buffers[ENCRYPTOR]->shouldSignal = false;
        pthread_mutex_unlock(&buffer.buffers[ENCRYPTOR]->lock);
    }
    if (buffer.buffers[OUT_COUNTER]->shouldSignal) {
        buffer.buffers[OUT_COUNTER]->shouldSignal = false;
        pthread_mutex_unlock(&buffer.buffers[OUT_COUNTER]->lock);
    }
}

/**
 * Deallocates the currently allocated buffer
 */
void freeBuffer() {
    buffer.capacity = 0;
    for (int i = 0; i < NUM_THREADS; i++) if (buffer.buffers[i] != NULL) {
        sem_destroy(&buffer.buffers[i]->size);
        pthread_mutex_destroy(&buffer.buffers[i]->lock);
        pthread_cond_destroy(&buffer.buffers[i]->signal);

        while (buffer.buffers[i]->head != NULL) {
            struct Node* n = buffer.buffers[i]->head->next;
            free(buffer.buffers[i]->head);
            buffer.buffers[i]->head = n;
        }

        free(buffer.buffers[i]);
        buffer.buffers[i] = NULL;
    }
}

/**
 * Allocated a new buffer
 * @param size
 * size of the buffer
 */
void buffer_init(unsigned size) {
    for (int i = 0; i < NUM_THREADS; i++) buffer.buffers[i] = NULL;
    buffer.capacity = size;

    for (int t = 0; t < NUM_THREADS; t++) {
        struct Queue* q = calloc(1, sizeof(struct Queue));
        if (q == NULL) {
            freeBuffer();
            char msg[50];
            sprintf(msg, "failed to malloc buffer for %s", thread2string(t));
            printErrAndExit(msg, true, MALLOC_ERROR);
        }

        if (sem_init(&q->size, 0, t == READER ? size : 0)) {
            freeBuffer();
            free(q);
            printErrAndExit("failed to create mutex for queue", 1, SEMAPHORE_FAILURE);
        }

        if (pthread_mutex_init(&q->lock, NULL)) {
            sem_destroy(&q->size);
            freeBuffer();
            free(q);
            printErrAndExit("failed to create mutex for queue", 1, MUTEX_FAILURE);
        }

        if (pthread_cond_init(&q->signal, NULL)) {
            sem_destroy(&q->size);
            pthread_mutex_destroy(&q->lock);
            freeBuffer();
            free(q);
            printErrAndExit("failed to create condition variable for queue", 1, CONDITION_FAILURE);
        }

        q->head = NULL;
        q->tail = NULL;
        q->s = t == READER ? size : 0;
        q->shouldSignal = false;

        buffer.buffers[t] = q;
    }
}

/**
 * Removes and returns the head of the specified thread's queue
 * @param b
 * thread to pop the queue for
 * @return
 * letter stored in the buffer
 */
int pop(enum THREADS b) {
    struct Queue* q = buffer.buffers[b];
    int c = EOF;

    sem_wait(&q->size);
    if (b == READER) {
        q->s--;
        return 0;
    }

    pthread_mutex_lock(&q->lock);

    struct Node* n = q->head;
    if (n->next == NULL) {
        q->head = NULL;
        q->tail = NULL;
    } else q->head = n->next;

    c = n->c;

    free(n);

    q->s--;
    pthread_mutex_unlock(&q->lock);

    return c;
}

/**
 * Pushes a letter onto the queue to be popped next from
 * @param b
 * thread doing the push
 * @param n
 * letter to push
 */
void push(enum THREADS b, int n) {
    enum THREADS next;
    switch (b) {
        case READER:
            next = IN_COUNTER;
            break;
        case IN_COUNTER:
            next = ENCRYPTOR;
            break;
        case ENCRYPTOR:
            next = OUT_COUNTER;
            break;
        case OUT_COUNTER:
            next = WRITER;
            break;
        case WRITER:
            if (n == EOF) {
                freeBuffer();
                return;
            }
            next = READER;
            break;
        default: return;
    }

    struct Queue* q = buffer.buffers[next];
    pthread_mutex_lock(&q->lock);

    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        freeBuffer();
        printErrAndExit("failed to alloc node for buffer", true, MALLOC_ERROR);
    }

    node->next = NULL;
    node->c = n;

    if (q->head == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }

    q->s++;

    pthread_mutex_lock(&buffer.buffers[b]->lock);
    if (buffer.buffers[b]->shouldSignal) {
        int semVal;
        sem_getvalue(&buffer.buffers[b]->size, &semVal);
        if (semVal == 0) pthread_cond_signal(&buffer.buffers[b]->signal);
    }
    pthread_mutex_unlock(&buffer.buffers[b]->lock);

    pthread_mutex_unlock(&q->lock);
    sem_post(&q->size);
}
