#ifndef GLOBAL_H
#define GLOBAL_H

#include "queue.h"

typedef struct SystemQueue{
    Queue8 *timer;
    Queue8 *com1_in;
    Queue8 *com1_out;
} SystemQueue;

#endif
