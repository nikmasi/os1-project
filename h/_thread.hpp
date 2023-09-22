//
// Created by os on 8/4/23.
//

#ifndef PROJECT_BASE__THREAD_HPP
#define PROJECT_BASE__THREAD_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/riscv.hpp"
#include "MemoryAllocator.hpp"
#include "../h/scheduler.hpp"


class _thread{
public:
    enum ThreadState {CREATED, READY, RUNNING, BLOCKED, FINISHED,SLEEPING};

    ~_thread() { delete[] stack;}

    using Body = void (*)(void*);

    bool isStateFinished();
    void setStateFinished();
    void setState(ThreadState state);

    static uint64 getTimeSliceCounter();
    static void setTimeSliceCounter(uint64 timeSliceCounter);

    static void yield();
    int start();

    static _thread* initMain();
    static _thread *createThread(Body body, void* arg);
    static _thread *createThread(Body body, void* arg,uint64* stack);
    static _thread* threadInit(Body body, void* arg);
    static _thread* initIdle();

    static void idleJob(void*);

    void *operator new (size_t size);
    void *operator new[] (size_t size);
    //void operator delete (void* p);
    //void operator delete[] (void* p);

    static _thread* main;
    static _thread* idle;
    static _thread *running;

    static uint64 timeSliceCounter;

    static bool finished;
    static uint64 sId;

    friend class Riscv;

    uint64 getId()const{ return id;}

    static void dispatch();

    int _threadStart(thread_t myHandle){Scheduler::put(myHandle);return 0;}
private:
    _thread(Body body, void* arg) :
            body(body),
            arg(arg),
            stack(body != nullptr ? (uint64 *)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64)) : nullptr),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            thread_state(CREATED),
            timeSlice(DEFAULT_TIME_SLICE)
    {
        id=++sId;
    }
    _thread(Body body,void* arg,uint64* stack):
            body(body),
            arg(arg),
            stack(stack),
            context({body != nullptr ? (uint64) &threadWrapper: 0,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            thread_state(CREATED),
            timeSlice(DEFAULT_TIME_SLICE)
    {
        id=++sId;
    }

    _thread(Body body,void* arg,uint64* stack,uint64 timeSlice):
            body(body),
            arg(arg),
            stack(stack),
            context({body != nullptr ? (uint64) &threadWrapper: 0,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            thread_state(CREATED),
            timeSlice(timeSlice)
    {
        id=++sId;
    }


    struct Context{ uint64 ra;  uint64 sp;};

    Body body;
    void *arg;
    uint64 *stack;
    Context context;
    ThreadState thread_state;
    uint64 timeSlice;
    uint64 id;

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static int exit();

    static void threadWrapper();

    static void join(_thread** handle);

    static uint64 constexpr STACK_SIZE = 8*DEFAULT_STACK_SIZE;
};

#endif //PROJECT_BASE__THREAD_HPP