#ifndef PROJECT_BASE_V1_1_CONSOLE_HPP
#define PROJECT_BASE_V1_1_CONSOLE_HPP

#include "Sem.hpp"
#include "buffer.hpp"
#include "MemoryAllocator.hpp"

class console
{
private:

public:
    buffer getcBuffer;
    buffer putcBuffer;
    static console& getInstance();
    void getcBufferPut(char c);
    char getcBufferGet();
    void putcBufferPut(char c);
    char putcBufferGet();
    void prepConosle();
};

#endif
