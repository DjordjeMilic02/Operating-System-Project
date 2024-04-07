#include "../lib/hw.h"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/console.hpp"

extern void userMain();

void userMainActivation(void* )
{
    userMain();
}

void putcActivation(void* )
{
    while(true)
    {
        while(*((char*)(CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT)
        {
            char c = console::getInstance().putcBufferGet();
            *(char*)CONSOLE_TX_DATA = c;
        }
    }
}

void doNothing(void*)
{
    while(true)
    {
        thread_dispatch();
    }
}

void main()
{
    __asm__ volatile ("csrw stvec, %[vec]" : : [vec]"r" ((uint64)&riscv::supervisorTrap | 1));
    console::getInstance().prepConosle();

    TCB* t;
    thread_create(&t, nullptr, nullptr);
    t -> system = true;
    TCB::running = t;

    TCB* user;
    thread_create(&user, userMainActivation, nullptr);


    TCB* idle;
    thread_create(&idle, doNothing, nullptr);
    idle -> system = true;

    TCB* putter;
    thread_create(&putter, putcActivation, nullptr);
    putter -> system = true;

    riscv::ms_sstatus(riscv::SSTATUS_SIE);
    thread_join(user);


    while(console::getInstance().putcBuffer.count > 0)
    {
        thread_dispatch();
    }

}