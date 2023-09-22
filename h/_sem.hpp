//
// Created by os on 8/6/23.
//

#ifndef PROJECT_BASE__SEM_HPP
#define PROJECT_BASE__SEM_HPP

#include "list.hpp"
#include "../lib/mem.h"
#include "_thread.hpp"

class _sem{
public:
    _sem(unsigned init):val(init),init(init),closed(false){}

    ~_sem();
    void wait();
    void signal();

    int value()const {return val;}

    static void createSemaphore(sem_t* handle, unsigned init);

    void *operator new (size_t size);
    void operator delete (void* p);

    int close();
protected:
    void block();
    void unblock();

private:
    long int val;
    unsigned init;

    bool closed;
    List blocked;
};

#endif //PROJECT_BASE__SEM_HPP
