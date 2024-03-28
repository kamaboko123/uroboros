;void context_switch(Context **old_context, Context *new_context)
global context_switch
context_switch:
    mov eax, [esp + 4] ;old
    mov edx, [esp + 8] ;new
    
    ;struct Context
    push ebp
    push ebx
    push esi
    push edi

    ;switch task
    ;save old context
    mov [eax], esp
    ;restore new context
    mov esp, edx
    
    pop edi
    pop esi
    pop ebx
    pop ebp

    ret

;void start_mtask(Context *sched_context)
global start_mtask
start_mtask:
    mov eax, [esp + 4]
    mov esp, eax
    
    pop edi
    pop esi
    pop ebx
    pop ebp
    
    ret

