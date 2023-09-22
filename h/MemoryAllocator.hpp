//
// Created by os on 8/6/23.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct FreeMem {
    FreeMem *next;
    FreeMem *prev;
    size_t size;
};
struct PCBMem {
    PCBMem *next;
    PCBMem *prev;
    size_t size;
};

class MemoryAllocator {
private:
    MemoryAllocator() {}
    ~MemoryAllocator() {}

    static int tryToJoin(FreeMem *);

public:
    static FreeMem *free_mem_head; //treba u priv
    static PCBMem *pcb_mem_head;  //i ovo

    MemoryAllocator(const MemoryAllocator &) = delete;

    static void initMemorySpace();
    static void *mem_alloc(size_t size);
    static int mem_free(void *);
};
#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
