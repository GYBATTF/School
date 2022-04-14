#ifndef BUFFER_H
#define BUFFER_H

#include "../threads/threads.h"

void buffer_init(unsigned size);
int pop(enum THREADS b);
void push(enum THREADS b, int c);

#endif
