#ifndef __EASY_TRACE_H__
#define __EASY_TRACE_H__

#ifndef __KATRACK_BACK_H__
#define __KATRACK_BACK_H__

#include <stdlib.h>
#include <stdio.h>
#include <cxxabi.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>

void Backtrace()
{
    unw_cursor_t cursor;
    unw_context_t context;

    // context 封装了当前机器状态的参数值，例如不同寄存器的值等
    // cursor 相当于是栈帧的迭代器
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // 一帧一帧地回溯，直到没有可用父栈帧
    while (unw_step(&cursor) > 0)
    {
        unw_word_t offset, pc;
        // IP (instruction pointer)跟 PC、SP(Stack
        // Pointer)可以简单理解为同一个东西
        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (pc == 0)
        {
            break;
        }
        printf("0x%lx: ", pc);

        char sym[256];
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0)
        {
            char* nameptr = sym;
            int status;
            char* demangled =
                abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if (status == 0)
            {
                nameptr = demangled;
            }
            printf(" (%s+0x%lx)\n", nameptr, offset);
            free(demangled);
        }
        else
        {
            printf(" -- error: unable to obtain symbol name for this frame\n");
        }
    }
}

#endif

#endif