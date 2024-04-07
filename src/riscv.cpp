#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/Sem.hpp"
#include "../h/console.hpp"
#include "../test/printing.hpp"

void riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    if(TCB::running -> system)
    {
        __asm__ volatile ("csrs sstatus, %[temp]" : : [temp] "r" (0x100));
    }
    else
    {
        __asm__ volatile ("csrc sstatus, %[temp]" : : [temp] "r" (0x100));
    }
    __asm__ volatile ("sret");
}

void riscv::handleSupervisorTrapx()
{
    uint64 volatile scause = r_scause();
    uint64 volatile sepc = r_sepc();

    if(scause == 0x08 || scause == 0x09)
    {
        uint64 x;
        uint64 volatile sstatus = r_sstatus();
        __asm__ volatile ("mv %[arg0], a0" : [arg0]"=r"(x));

        if(x == 0x01)
        {
            uint64 arg;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (arg));

            void* addr = MemoryAllocator::getInstance().mem_alloc(arg);
            __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (addr));
        }
        if(x == 0x02)
        {
            void* arg;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r"(arg));

            int res = MemoryAllocator::getInstance().mem_free((void*)arg);
            __asm__ volatile ("mv a0, %[ret]" : : [ret]"r"(res));
        }
        if(x == 0x11)
        {
            TCB** t;
            void(*s)(void*);
            void* fArg;
            uint64 res = 0;
            __asm__ volatile ("mv %[arg2], a3" : [arg2]"=r"(fArg));
            __asm__ volatile ("mv %[arg1], a2" : [arg1]"=r"(s));
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r"(t));

            uint64* stack = (uint64*) MemoryAllocator::getInstance().mem_alloc((DEFAULT_STACK_SIZE*8 + MEM_BLOCK_SIZE -1)/MEM_BLOCK_SIZE);
            TCB::createThread((TCB::fBody)s, (TCB**)t, fArg, stack);

            if((TCB**)t == nullptr)
            {
                res--;
            }

            __asm__ volatile ("mv a0, %[ret]" : : [ret]"r"(res));
        }
        if(x == 0x12)
        {
            TCB::running -> setFinished(true);
            MemoryAllocator::getInstance().mem_free(TCB::running -> stack);
            TCB::dispatch();
        }
        if(x == 0x13)
        {
            TCB::dispatch();
        }
        if(x == 0x14)
        {
            TCB* t;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r"(t));
            TCB::join(t);
        }
        if(x == 0x21)
        {
            Sem** sem;
            int i;
            uint64 ret = 0;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (sem));
            __asm__ volatile ("mv %[arg1], a2" : [arg1]"=r" (i));
            Sem::openSemaphore(i, (Sem**)sem);

            if(sem == nullptr)
            {
                ret--;
            }

            __asm__ volatile ("mv a0, %[res]" : : [res]"r"(ret));
        }
        if(x == 0x22)
        {
            Sem* sem;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (sem));

            uint64 ret = Sem::closeSemaphore(sem);

            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (ret));
        }
        if(x == 0x23)
        {
            Sem* sem;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (sem));

            uint64 ret = Sem::SemaphoreWait(sem);

            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (ret));
        }
        if(x == 0x24)
        {
            Sem* sem;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (sem));

            uint64 ret = Sem::SemaphoreSignal(sem);

            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (ret));
        }
        if(x == 0x31)
        {
            time_t arg;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (arg));

            int ret = TCB::sleep(arg);

            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (ret));
        }
        if(x == 0x41)
        {
            char c = console::getInstance().getcBufferGet();
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (c));
        }
        if(x == 0x42)
        {
            uint64 c;
            __asm__ volatile ("mv %[arg0], a1" : [arg0]"=r" (c));
            console::getInstance().putcBufferPut(c);
        }

        w_sstatus(sstatus);
        w_sepc(sepc + 4);
    }
}

void riscv::handleSupervisorTrapy()
{
    mc_sip(SIP_SSIP);

    TCB::timeCounter++;
    if(TCB::head != nullptr)
    {
        for(sleepy* temp = TCB::head; temp != nullptr;)
        {
            temp -> periodsLeft--;
            if(temp -> periodsLeft <= 0)
            {
                TCB* nov = temp -> elem;
                nov -> sleeping = false;
                Scheduler::getInstance().put(nov);
                sleepy* extratemp = TCB::head;
                TCB::head = TCB::head -> next;

                if(temp -> next == nullptr)
                {
                    break;
                }

                temp = temp -> next;
                MemoryAllocator::getInstance().mem_free(extratemp);

                continue;
            }
            if(temp -> next == nullptr)
            {
                break;
            }

            temp = temp -> next;
        }
    }

    if (TCB::timeCounter >= TCB::running->getTime())
    {

        uint64 volatile sepc1 = r_sepc();
        uint64 volatile sstatus1 = r_sstatus();
        TCB::timeCounter = 0;

        TCB::dispatch();

        w_sstatus(sstatus1);
        w_sepc(sepc1);
    }
}

void riscv::handleSupervisorTrapz()
{
    int i = plic_claim();
    if(i == CONSOLE_IRQ)
    {
        while(*((char*)(CONSOLE_STATUS)) & CONSOLE_RX_STATUS_BIT)
        {
            char c = (*(char*)CONSOLE_RX_DATA);
            console::getInstance().getcBufferPut(c);
        }
    }
    plic_complete(CONSOLE_IRQ);
}