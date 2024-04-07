#include "../h/syscall_c.h"

void* mem_alloc(size_t sz)
{
    uint64 x;
    size_t param = (sz + MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    //size_t param = sz;
    __asm__ volatile ("mv a1, %[op]" : : [op]"r" (param));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[rx1], a0" : [rx1]"=r" (x));
    return (void*) x;
}

int mem_free(void* addr)
{
    void* adresa = addr;
    int x;
    __asm__ volatile ("mv a1, %[op]": : [op]"r" (adresa));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[rx12], a0" : [rx12]"=r" (x));
    return x;
}

int thread_create(TCB** thread, void(*f)(void*), void* fArg)
{
    void* args = fArg;
    void(*sr)(void*) = f;
    TCB** t = thread;
    int x;

    __asm__ volatile ("mv a3, %[args]": :[args]"r" (args));
    __asm__ volatile ("mv a2, %[start]": : [start]"r" (sr));
    __asm__ volatile ("mv a1, %[t]" : : [t]"r" (t));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[re], a0" : [re]"=r" (x));
    return x;
}

void thread_join(TCB* t)
{
    TCB* th = t;

    __asm__ volatile ("mv a1, %[thr]" : : [thr]"r" (th));
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile ("ecall");
}

int thread_exit()
{
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
    return 0;
}

void thread_dispatch()
{
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

int time_sleep(time_t t)
{
    time_t tt= t;
    int t1;
    __asm__ volatile ("mv a1, %[ttt]" : : [ttt]"r" (tt));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[x1], a0" : [x1]"=r" (t1));
    return t1;
}

int sem_open(Sem** sem, unsigned int i)
{
    unsigned int i1 = i;
    int x;
    Sem** s = sem;
    __asm__ volatile ("mv a2, %[i2]" : : [i2]"r" (i1));
    __asm__ volatile ("mv a1, %[s1]" : : [s1]"r" (s));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[x1], a0" : [x1]"=r" (x));
    return x;
}

int sem_close(Sem* sem)
{
    int x;
    Sem* s = sem;
    __asm__ volatile ("mv a1, %[s1]" : : [s1]"r" (s));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[x1], a0" : [x1]"=r" (x));
    return x;
}

int sem_wait(Sem* sem)
{
    int x;
    Sem* s = sem;
    __asm__ volatile ("mv a1, %[s1]" : : [s1]"r" (s));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[x1], a0" : [x1]"=r" (x));
    return x;
}

int sem_signal(Sem* sem)
{
    int x;
    Sem* s = sem;
    __asm__ volatile ("mv a1, %[s1]" : : [s1]"r" (s));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[x1], a0" : [x1]"=r" (x));
    return x;
}

char getc()
{

    char x;
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[re], a0" : [re]"=r" (x));
    return x;
}

void putc(char c)
{

    char ch = c;
    __asm__ volatile ("mv a1, %[cha]" : : [cha]"r" (ch));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");

}




