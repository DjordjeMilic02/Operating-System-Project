#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct FreeMem
{
    FreeMem* prev;
    FreeMem* next;
    size_t size;
};

class MemoryAllocator
{
private:
    FreeMem *head = nullptr;

public:
    static MemoryAllocator& getInstance();
    void* mem_alloc(size_t size);
    int mem_free(void* addr);

};

#endif
