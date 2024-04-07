#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "MemoryAllocator.hpp"

struct sleepy
{
    TCB *elem;
    uint64 periodsLeft;
    sleepy *next;
};


class TCB
{
private:

    static sleepy* head;

    friend class riscv;
    friend class Sem;

    static uint64 timeCounter;

    using fBody = void(*)(void*);

    struct context
    {
        uint64 ra;
        uint64 sp;
    };
    context context1;

    uint64 timeSlice;
    fBody fbody;
    void* funcArg;
    uint64* stack;
    static TCB* createCore(fBody body, void* funcArg, uint64* stack);

    static void threadWrapper();
    static void contextSwitch(context *oldC, context *newC);
    static void dispatch();

public:

    TCB(){}

    bool blocked;
    bool finished;
    bool sleeping;
    bool system;

    static TCB* running;

    bool isFinished();
    void setFinished(bool b);

    static TCB* createThread(fBody body, TCB** t, void* funcArg, uint64* stack);
    static void join(TCB* t);
    static int sleep(time_t time);

    uint64 getTime();

};

#endif
