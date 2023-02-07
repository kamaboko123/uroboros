#include "queue.h"

//メモリの配置までお任せでとりあえず使いたい用
Queue8 *q8_make(uint32_t size, uint8_t default_value){
    Queue8 *mem = (Queue8 *)vmalloc(sizeof(Queue8) + sizeof(uint8_t) * size);
    //先頭をキューの管理用、後ろをバッファにする
    q8_init(mem, (uint8_t *)(mem) + sizeof(Queue8), size, default_value);
    
    return mem;
}

void q8_init(Queue8 *q, uint8_t *buf, uint32_t buf_size, uint8_t default_value){
    q->size = buf_size - 1;
    q->buf = buf;
    q->head = 0;
    q->tail = 0;
    q->drop = 0;
    q->default_value = default_value;
}

void q8_in(Queue8 *q, uint8_t data){
    if(((q->tail + 1) % q->size) + 1 == q->head){
        //キューがいっぱい
        q->drop += 1;
        return;
    }
    q->tail += 1;
    q->tail %= q->size;
    q->buf[q->tail] = data;
}

uint8_t q8_de(Queue8 *q){
    if(q->head == q->tail){
        return q->default_value;
    }
    uint8_t data = q->buf[q->head];
    q->head += 1;
    q->head %= q->size;
    return data;
}

uint32_t q8_usage(Queue8 *q){
    if(q->head <= q->tail){
        return q->tail - q->head;
    }
    return (q->size - q->head) + q->tail;
}
