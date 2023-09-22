//
// Created by os on 8/3/23.
//


#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/_thread.hpp"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"



extern void userMain();

bool ffinishedA = false;
bool ffinishedB = false;
bool ffinishedC = false;
thread_t threads[3];

void workerBodyA(void* arg) {
    for (uint64 i = 0; i < 2; i++) {
        printstring("A: i="); printinteger(i); printstring("\n"); printinteger(_thread::running->getId()); printstring(" ");
        printhexadecimal((uint64)_thread::running);printstring(" ");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
        }
    }
    printstring("A finished!\n");
    ffinishedA = true;
}

void workerBodyB(void* arg) {
    for (uint64 i = 0; i < 2; i++) {
        printstring("B: i="); printinteger(i); printstring("\n"); printinteger(_thread::running->getId());printstring(" ");
        printhexadecimal((uint64)_thread::running);printstring(" ");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
        }
    }
    printstring("B finished!\n");
    ffinishedB = true;
}

void workerBodyC(void* arg) {
    for (uint64 i = 0; i < 2; i++) {
        printstring("C: i="); printinteger(i); printstring("\n"); printinteger(_thread::running->getId());
        printstring(" ");printhexadecimal((uint64)_thread::running);printstring(" ");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
        }
    }
    printstring("C finished!\n");
    ffinishedC = true;
}

void test1(){
    printstring("main:  ");
    printhexadecimal((uint64)_thread::main);
    printstring("  ");
    printinteger(_thread::main->getId());
    printstring("  \n");
    printstring("idle:  ");
    printhexadecimal((uint64)_thread::idle);
    printstring("  ");
    printinteger(_thread::idle->getId());
    printstring("\n");
    thread_create(&threads[0], workerBodyA, nullptr);
    printstring("ThreadA created  ");
    printhexadecimal((uint64)threads[0]);printstring("  ");
    printinteger(threads[0]->getId());
    printstring("  \n");

    thread_create(&threads[1], workerBodyB, nullptr);
    printstring("ThreadB created  ");printhexadecimal((uint64)threads[1]);printstring("  ");
    printinteger(threads[1]->getId());
    printstring("\n");

    thread_create(&threads[2], workerBodyC, nullptr);
    printstring("ThreadC created  ");printhexadecimal((uint64)threads[2]);printstring("  ");
    printinteger(threads[2]->getId());
    printstring("\n");

    printstring("\nprintlist\n");
    Scheduler::printScheduler();
    printstring("\nkrajprintlist\n");
    while (!(ffinishedB && ffinishedC)) {
        thread_dispatch();
    }

    printstring("\nprintlist\n");
    Scheduler::printScheduler();
    printstring("\nkrajprintlist\n");


    delete _thread::main;
    delete _thread::idle;
    printstring("\n\nkraj\n\n");
}

void main(){

    uint64 trap=(uint64)(&Riscv::supervisorTrap)|1;
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r"(trap));
    Riscv::mc_sstatus(1<<1);
    MemoryAllocator::initMemorySpace();

    _thread::running= _thread::initMain();

    _thread::idle=_thread::initIdle();
    nonKernelMode();

    userMain();

    //test1();
}




