#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../lib/mem.h"

node* Scheduler::head = nullptr;
node* Scheduler::tail = nullptr;

Scheduler& Scheduler::getInstance()
{
    static Scheduler instance;
    return instance;
}

TCB* Scheduler::get()
{
    if(head == nullptr)
    {
        return nullptr;
    }

    node* node1 = head;
    head = head -> next;

    if(head == nullptr)
    {
        tail = nullptr;
    }
    TCB* t = node1 -> elem;
    MemoryAllocator::getInstance().mem_free(node1);
    //__mem_free(node1);
    return t;

}

void Scheduler::put(TCB* t)
{
    node* curr = (node*) MemoryAllocator::getInstance().mem_alloc((sizeof(node) + MEM_BLOCK_SIZE -1)/MEM_BLOCK_SIZE);
    //node* curr = (node*) __mem_alloc(sizeof(node));
    curr -> elem = t;
    curr -> next = nullptr;

    if(head == nullptr)
    {
        head = curr;
        tail = curr;
    }
    else
    {
        tail -> next = curr;
        tail = tail -> next;
    }
}
