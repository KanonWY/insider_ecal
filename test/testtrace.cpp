#include "esaytrace.h"

void dudiaujdu() { Backtrace(); }

void k() { dudiaujdu(); }

void foo(int a, int b) { k(); }

void m() { foo(1, 2); }

int main()
{
    m();
    return 0;
}