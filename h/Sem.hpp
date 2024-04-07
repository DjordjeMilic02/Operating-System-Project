#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP

#include "../lib/hw.h"
#include "../h/TCB.hpp"

struct blocked
{
    TCB* t;
    blocked* next;
};

class Sem
{
private:
    int i;

    blocked* head = nullptr;
    blocked* tail = nullptr;

    TCB* getT();
    void putT(TCB* t);

public:
    static void openSemaphore(int i, Sem** sem);
    static uint64 closeSemaphore(Sem* sem);
    static uint64 SemaphoreWait(Sem* sem);
    static uint64 SemaphoreSignal(Sem* sem);
};

#endif
