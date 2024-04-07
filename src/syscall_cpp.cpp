#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"

void* operator new(size_t sz)
{
    return mem_alloc(sz);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void* addr)
{
    mem_free(addr);
}

void operator delete[](void *p)
{
    mem_free(p);
}

Thread::Thread(void (*fBody)(void *), void *fArg)
{
    t = nullptr;
}

int Thread::start()
{
    thread_create(&t, &threadWrapper, (void*)this);
    return 0;
}

void Thread::dispatch()
{
    thread_dispatch();
}

Thread::~Thread()
{
    t->setFinished(true);
    delete t;
}

Thread::Thread()
{
    t = nullptr;
}

void Thread::threadWrapper(void *t1)
{
    ((Thread*)t1) -> run();
}

int Thread::sleep(unsigned long t)
{
    return time_sleep(t);
}

PeriodicThread::PeriodicThread(time_t period1)
{
    period = period1;
}

void PeriodicThread::run()
{
    while(true)
    {
        periodicActivation();
        sleep(period);
    }
}

void PeriodicThread::terminate()
{
    delete this;
}

Semaphore::Semaphore(unsigned int i)
{
    sem_open(&sem,i);
}

int Semaphore::wait()
{
    return sem_wait(sem);
}

int Semaphore::signal()
{
    return sem_signal(sem);
}

Semaphore::~Semaphore()
{
    sem_close(sem);
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}
