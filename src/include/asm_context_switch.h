#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

typedef struct Context{
    //low address(stack top)
    //eaxやebxはここでは保持不要
    //context_switchが呼ばれる際に（必要であれば）スタックに保存されている
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
} Context;

void context_switch(struct Context **old_context, struct Context *new_context);
void start_mtask(Context *sched_context);

#endif
