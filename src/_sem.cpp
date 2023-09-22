//
// Created by os on 8/6/23.
//
#include "../h/_sem.hpp"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

void _sem::wait() {

    if(closed) return;

    if (--val < 0) {
        _thread *old = _thread::running;
        old->setState(_thread::BLOCKED);
        blocked.addLast(old);
        thread_dispatch();
        _thread::dispatch();
        //block();
    }
}

void _sem::signal() {
    if(closed) return;

    if (++val <= 0) {unblock();}
}

void _sem::block() {
    _thread *old = _thread::running;
    old->setState(_thread::BLOCKED);
    blocked.addLast(old);

    thread_dispatch();
}

void _sem::unblock() {
    _thread* t= blocked.removeFirst();
    t->setState(_thread::READY);
    Scheduler::put(t);
}

void *_sem::operator new(size_t size) {
    return MemoryAllocator::mem_alloc((size % MEM_BLOCK_SIZE == 0) ? size / MEM_BLOCK_SIZE :1 + size / MEM_BLOCK_SIZE);
}

void _sem::operator delete(void *p) {
    MemoryAllocator::mem_free(p);
}

void _sem::createSemaphore(sem_t * handle, unsigned init) {
    *handle= new _sem(init);
}
_sem::~_sem() {close();}

int _sem::close() {
    return 0;
    if(closed) return -1;
    closed = true;
    while(blocked.peekLast())
    {
        thread_t tek = blocked.removeFirst();
        tek->setState(_thread::READY);
        Scheduler::put(tek);

    }
    return 0;
}
