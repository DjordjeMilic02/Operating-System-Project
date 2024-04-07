This project is a completely functional operating system kernel complete with two modes, synchronous and asynchronous context switching, a console, a cheduler and support for multiple threads and their synchronization. It is written for RISCV architecture, variant RV64IMA, in C/C++ and assembly. The system is written in a way there a user can execute their custom C++ program which can use the following services:
1. void* ::operator new(size_t size) (allocates memmory)
2. void ::operator delete(void* address) (deallocates memory)
3. Thread (void (*body)(void*), void* arg) (makes a new thread)
4. ~Thread() (Destroys a thread)
5. int start() (stars a new Thread)
6. void join(Thread t) (disables the caller Thread from continuing before the Thread passed as an argument completes)
7. static void dispatch() (context switch)
8. static int sleep(time_t time) (disables the current Thread for a specief amount of time)

9. Semaphore (unisgned int i = 1) (creates a new Semaphore)
10. virtual ~Semaphore() (destroys a Semaphore)
11. int wait() (blocks the current Thread at the caller Semaphore if Semaphore has no more tickets to give)
12. int signal() (gives the caller Semaphore a ticket)

13. PeriodicThread(time_t period) (creates a periodic Thread, a Thread that only executes at a specified time interval)
14. void terminate() (stops PeriodicThread activation)

15. static char gets() (returns a character from the console)
16. static void putc(char c) (prints a character to the console)
