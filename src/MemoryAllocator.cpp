#include "../h/MemoryAllocator.hpp"
#include "../test/printing.hpp"

MemoryAllocator& MemoryAllocator::getInstance()
{
    static MemoryAllocator instance;
    return instance;
}

void* MemoryAllocator::mem_alloc(size_t size1)
{
    static bool first = false;

    if(first == false)
    {
        first = true;
        head = (FreeMem*)(HEAP_START_ADDR);
        head -> prev = nullptr;
        head -> next = nullptr;
        head -> size = (uint64)(((uint64*)HEAP_END_ADDR - (uint64*)HEAP_START_ADDR - sizeof(MemoryAllocator))/MEM_BLOCK_SIZE);
    }

    void* res = nullptr;

    for(FreeMem* temp = head; temp != nullptr; temp = temp -> next)
    {
        if(temp -> size >= size1)
        {
            res = temp;
            if(temp -> size == size1)
            {
                temp -> size = size1;

                if(temp -> prev == nullptr)
                {
                    head = temp -> next;
                }
                else
                {
                    temp -> prev -> next = temp -> next;
                }
                if(temp -> next != nullptr)
                {
                    temp -> next -> prev = temp -> prev;
                }
                break;
            }
            else
            {
                FreeMem* node = (FreeMem*)((uint64*) temp + size1 * MEM_BLOCK_SIZE);

                if(temp -> prev != nullptr)
                {
                    temp -> prev -> next = node;
                }
                else
                {
                    head = node;
                }
                if(temp -> next != nullptr)
                {
                    temp -> next -> prev = node;
                }

                node -> prev = temp -> prev;
                node -> next = temp -> next;
                node -> size = temp -> size - size1;
                temp -> size = size1;
                break;
            }
        }
    }

    return (void*)((uint64*) res + sizeof(FreeMem));
}

int MemoryAllocator::mem_free(void* addr) {
    if ((uint64 *) addr > (uint64 *) HEAP_END_ADDR) return -1;
    if ((uint64 *) addr < (uint64 *) HEAP_START_ADDR) return -1;
    if (head == nullptr || (uint64 *) addr > (uint64 *) head) return -2;

    FreeMem *last = nullptr;
    if(head -> next != nullptr)
    {
        for (last = head; last->next != nullptr; last = last->next)
        {
            if ((uint64 *) addr > (uint64 *) (last -> next))
            {
                break;
            }
        }
    }
    else
    {
        last = head;
    }


    FreeMem *curr = (FreeMem *) ((uint64*) addr - sizeof(FreeMem));

    curr -> next = last -> next;
    curr -> prev = last;
    last -> next = curr;
    if(curr -> next != nullptr)
    {
        curr -> next -> prev = curr;
    }

    if(curr -> next != nullptr && (((char*)(curr -> next) + 8*(curr -> next -> size*MEM_BLOCK_SIZE)) >= (char*)curr))
    {
        curr -> next -> size += curr -> size;
        curr -> next -> prev = curr -> prev;
        if(curr -> prev != nullptr)
        {
            curr -> prev -> next = curr -> next;
        }
    }
    if(last -> next != nullptr && (((char*)(last -> next) + 8*(last -> next -> size*MEM_BLOCK_SIZE)) >= (char*)last))
    {
        int flag = 0;
        if(last == head)
        {
            flag++;
        }
        last -> next -> size += last -> size;
        last -> next -> prev = last -> prev;
        if(last -> prev != nullptr)
        {
            last -> prev -> next = last -> next;
        }
        if(flag == 1)
        {
            head = last -> next;
            curr -> prev = nullptr;
        }

    }

    return 0;
}



