#include "../h/buffer.hpp"

buffer::buffer()
{
    Sem::openSemaphore(0,&avail);
}

char buffer::get()
{
     Sem::SemaphoreWait(avail);
     char c = core[head];
     head++;
     head %= 256;
     count--;
     return c;
}

void buffer::put(char c)
{
    if(count < 256)
    {
        core[tail] = c;
        tail++;
        tail %= 256;
        count++;
        Sem::SemaphoreSignal(avail);
    }
}
