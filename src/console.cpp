#include "../h/console.hpp"

console& console::getInstance()
{
    static console instance;
    return instance;
}

char console::getcBufferGet()
{
    return getcBuffer.get();
}
void console::getcBufferPut(char c)
{
    getcBuffer.put(c);
}

char console::putcBufferGet()
{
    return putcBuffer.get();
}

void console::putcBufferPut(char c)
{
    putcBuffer.put(c);
}

void console::prepConosle()
{
    Sem::openSemaphore(0,&getcBuffer.avail);
    Sem::openSemaphore(0,&putcBuffer.avail);
}