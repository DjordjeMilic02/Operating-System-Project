#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H

#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/TCB.hpp"
#include "../h/Sem.hpp"

void* mem_alloc(size_t size);
int mem_free(void* addr);

typedef TCB* thread_t;
int thread_create(TCB** thread, void(*f)(void*), void* fArg);
void thread_join(TCB* t);
int thread_exit();
void thread_dispatch();
int time_sleep(time_t time);

typedef Sem* sem_t;
int sem_open(Sem** sem, unsigned int i);
int sem_close(Sem* sem);
int sem_wait(Sem* i);
int sem_signal(Sem* i);

char getc();
void putc(char c);


#endif
