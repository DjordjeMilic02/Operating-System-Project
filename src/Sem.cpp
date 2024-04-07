#include "../h/Sem.hpp"
#include "../lib/mem.h"
#include "../h/console.hpp"

void Sem::openSemaphore(int i, Sem** sem)
{
    Sem* sem1 = (Sem*) MemoryAllocator::getInstance().mem_alloc((sizeof(Sem) + MEM_BLOCK_SIZE -1)/MEM_BLOCK_SIZE);
    //Sem* sem1 = (Sem*) __mem_alloc((sizeof(Sem) + MEM_BLOCK_SIZE -1)/MEM_BLOCK_SIZE);
    sem1 -> i = i;
    sem1 -> head = nullptr;
    sem1 -> tail = nullptr;
    *sem = sem1;
}

uint64 Sem::closeSemaphore(Sem *sem)
{
    if(sem == nullptr)
    {
        return -1;
    }

    TCB* curr = sem->getT();
    while(curr != nullptr)
    {
        curr -> blocked = false;
        Scheduler::getInstance().put(curr);
        curr = sem->getT();
    }
    return 0;
}

void Sem::putT(TCB *t)
{
    blocked* node = (blocked*) MemoryAllocator::getInstance().mem_alloc((sizeof(blocked) + MEM_BLOCK_SIZE -1)/MEM_BLOCK_SIZE);
    //blocked* node = (blocked*) __mem_alloc(sizeof(blocked));
    node -> t = t;
    node -> next = nullptr;

    if(tail == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail -> next = node;
        tail = tail -> next;
    }
}

TCB* Sem::getT()
{
    if(head == nullptr)
    {
        return nullptr;
    }

    blocked* elem = head;
    head = head -> next;
    if (head == nullptr)
    {
        tail = nullptr;
    }
    TCB* curr = elem -> t;

    MemoryAllocator::getInstance().mem_free(elem);
    //__mem_free(elem);
    return curr;
}

uint64 Sem::SemaphoreWait(Sem *sem)
{
    if(sem == nullptr)
    {
        return -1;
    }

    (sem -> i)--;

    if(sem -> i <= -1)
    {
        sem->putT(TCB::running);
        TCB::running -> blocked = true;
        TCB::dispatch();
    }
    return 0;
}

uint64 Sem::SemaphoreSignal(Sem *sem)
{
    if(sem == nullptr)
    {
        return -1;
    }

    (sem -> i)++;

    if(sem -> i <= 0)
    {
        TCB* t = sem -> getT();
        t -> blocked = false;
        Scheduler::getInstance().put(t);
    }

    return 0;
}


