//
// Created by os on 8/4/23.
//
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"

void operator delete(void *ptr) { MemoryAllocator::mem_free(ptr);}//__mem_free(ptr); }

void *operator new(size_t size) { return MemoryAllocator::mem_alloc(size);}//return __mem_alloc(sz); }

void *operator new[](size_t size){ return MemoryAllocator::mem_alloc(size);}//return __mem_alloc(size); }

void operator delete[](void *p) noexcept { MemoryAllocator::mem_free(p);}//__mem_free(p); }

