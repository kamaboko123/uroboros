#include "queue.h"

//メモリの配置までお任せでとりあえず使いたい用
Queue8 *q8_make(uint32_t size, uint8_t default_value){
    Queue8 *mem = (Queue8 *)vmalloc(sizeof(Queue8) + sizeof(uint8_t) * size + 1);
    //先頭をキューの管理用、後ろをバッファにする
    q8_init(mem, (uint8_t *)(mem) + sizeof(Queue8), size + 1, default_value);
    
    memset((char *)mem->buf, 0, size);

    return mem;
}

void q8_init(Queue8 *q, uint8_t *buf, uint32_t buf_size, uint8_t default_value){
    q->size = buf_size + 1;
    q->buf = buf;
    q->head = 0;
    q->tail = 0;
    q->drop = 0;
    q->default_value = default_value;
}

void q8_in(Queue8 *q, uint8_t data){
    if(q8_full(q)){
        //キューがいっぱい
        q->drop += 1;
        return;
    }
    q->buf[q->tail] = data;
    q->tail++;
    q->tail %= q->size;
}

uint8_t q8_de(Queue8 *q){
    if(q8_empty(q)){
        return q->default_value;
    }
    uint8_t data = q->buf[q->head];
    q->head++;
    q->head %= q->size;
    return data;
}

uint32_t q8_usage(Queue8 *q){
    if(q->head <= q->tail){
        return q->tail - q->head;
    }
    return (q->size - q->head) + q->tail;
}

bool q8_empty(Queue8 *q){
    return q->tail == q->head;
}

bool q8_full(Queue8 *q){
    return (q->tail + 1) % q->size == q->head;
}
