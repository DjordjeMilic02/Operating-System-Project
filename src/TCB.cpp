#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/mem.h"

TCB* TCB::running = nullptr;
uint64 TCB::timeCounter = 0;

sleepy* TCB::head = nullptr;

TCB* TCB::createThread(fBody body, TCB **t, void *funcArg, uint64 *stack)
{
    TCB* ret = createCore(body, funcArg, stack);
    *t = ret;
    if(body != nullptr)
    {
        Scheduler::getInstance().put(*t);
    }
    return ret;
}

void TCB::join(TCB* t)
{
    while(!(t->finished))
    {
        TCB::dispatch();
    }
}

TCB* TCB::createCore(fBody body, void *funcArg, uint64 *stack)
{
    TCB* t = (TCB*)MemoryAllocator::getInstance().mem_alloc((sizeof(TCB)+ MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    //TCB* t = (TCB*)__mem_alloc(sizeof(TCB));
    t -> fbody = body;
    if(body == nullptr)
    {
        stack = nullptr;
    }
    else
    {
        t -> stack = stack;
    }

    uint64 a,b;
    a = (uint64) &threadWrapper;
    if(stack != nullptr)
    {
        b = (uint64) &stack[DEFAULT_STACK_SIZE];
    }
    else
    {
        b = 0;
    }

    t -> context1 ={a,b};
    t -> funcArg = funcArg;
    t -> blocked = false;
    t -> finished = false;
    t -> sleeping = false;
    t -> system = false;
    t -> timeSlice = DEFAULT_TIME_SLICE;

    return t;
}

void TCB::dispatch()
{
    if(running == nullptr)
    {
        return;
    }
    TCB* old = running;
    if(!(old -> isFinished()) && !(old -> blocked) && !(old -> sleeping))
    {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();
    if(running == nullptr)
    {
        return;
    }
    TCB::contextSwitch(&old->context1, &running->context1);
}

void TCB::threadWrapper()
{
    riscv::popSppSpie();
    running -> fbody(running -> funcArg);
    running ->setFinished(true);
    thread_exit();
}

bool TCB::isFinished()
{
    return running -> finished;
}

void TCB::setFinished(bool b)
{
    running -> finished = b;
}

uint64 TCB::getTime()
{
    return timeSlice;
}

int TCB::sleep(time_t time)
{
    if(time != 0)
    {
        TCB::running -> sleeping = true;
        sleepy* node = (sleepy*)MemoryAllocator::getInstance().mem_alloc((sizeof(sleepy)+ MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
        //sleepy* node = (sleepy*)__mem_alloc(sizeof(sleepy));
        node -> elem = TCB::running;
        node -> periodsLeft = time;
        node -> next = nullptr;
        if(head == nullptr)
        {
            head = node;
        }
        else
        {
            if(head -> periodsLeft > node -> periodsLeft)
            {
                node -> next = head;
                head = node;
            }
            else
            {
                sleepy* temp;
                for(temp = head; temp != nullptr; temp = temp -> next)
                {
                    if(temp -> next == nullptr)
                    {
                        break;
                    }
                    if(temp -> next -> periodsLeft > node -> periodsLeft)
                    {
                        break;
                    }
                }
                node -> next = temp -> next;
                temp -> next = node;
            }
        }
        TCB::dispatch();
    }

    return 0;
}


