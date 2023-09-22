//
// Created by os on 8/4/23.
//
#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/scheduler.hpp"
#include "../h/_thread.hpp"

//----------THREAD----------
Thread::~Thread() {
    mem_free((void*)myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t) {
    return 0; //nije implementirano
}

void Thread::wrapper(void *ptr) {
    ((Thread*)ptr)->run();
}

int Thread::start() {
    myHandle->_threadStart(myHandle);//prelazak u kod kernela
    return 0;
}

void Thread::join() {
    //nije implementirano
}

Thread::Thread() {
    thread_create_noStart(&myHandle,wrapper,this);
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create(&myHandle,body,arg);
}

//----------SEMAPHORE----------
Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

//----------CONSOLE----------
char Console::getc() {return 0;}
void Console::putc(char) {return;}