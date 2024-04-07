#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "../h/syscall_c.h"

void* operator new (size_t s);
void* operator new[](size_t s);

void operator delete (void* addr);
void operator delete[] (void* addr);

class Thread
{
public:
    Thread(void(*fBody)(void*), void* fArg);
    int start();
    static void dispatch();
    static int sleep(unsigned long t);
    virtual ~Thread();
    static void threadWrapper(void* t);

protected:
    Thread();
    virtual void run(){}

private:
    TCB* t;
};

class PeriodicThread : public Thread
{
private:
    time_t period;

protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation (){}
    void run() override;

public:
    void terminate ();

};

class Semaphore
{
public:
    explicit Semaphore(unsigned i = 1);
    int wait();
    int signal();
    virtual ~Semaphore();

private:
    Sem* sem;
};

class Console
{
public:
    static char getc();
    static void putc(char c);
};

#endif
