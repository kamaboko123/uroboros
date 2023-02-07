#ifndef QUEUE_H
#define QUEUE_H

#include "stdint.h"
#include "mem.h"

typedef struct Queue8{
    uint32_t size;
    uint8_t *buf;
    uint32_t head;
    uint32_t tail;
    uint32_t drop;
    uint8_t default_value;
} Queue8;

Queue8 *q8_make(uint32_t size, uint8_t default_value);
void q8_init(Queue8 *q, uint8_t *buf, uint32_t buf_size, uint8_t default_value);
void q8_in(Queue8 *q, uint8_t data);
uint8_t q8_de(Queue8 *q);
uint32_t q8_usage(Queue8 *q);
bool q8_empty(Queue8 *q);
#endif
