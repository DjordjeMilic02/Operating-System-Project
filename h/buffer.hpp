#ifndef PROJECT_BASE_V1_1_BUFFER_HPP
#define PROJECT_BASE_V1_1_BUFFER_HPP

#include "Sem.hpp"

class buffer
{
private:
    friend class console;

public:
    char core[256];

    int head = 0;
    int tail = 0;
    int count = 0;

    Sem* avail;

    buffer();

    void put(char c);
    char get();
};

#endif
