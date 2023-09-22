//
// Created by os on 8/4/23.
//
#include "../h/syscall_c.h"
#include "../h/numCode.hpp"
#include "../lib/mem.h"

void *mem_alloc(size_t size) {
    //return __mem_alloc(size);
    if(size==0)return nullptr;
    size_t s=size;//(size+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r" (s));
    __asm__ volatile ("mv a0, %0" : : "r" (MEM_ALLOC));
    __asm__ volatile ("ecall");
    void * ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (void*)ret;
}

int mem_free(void * p) {
    //if(p)return __mem_free(p);
    if(!p) return -1;
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (MEM_FREE));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit() {
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_EXIT));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

void thread_dispatch() {
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
    return;
}

void thread_join(thread_t handle) {
    __asm__ volatile ("mv a3, a2");
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_JOIN));
    __asm__ volatile ("ecall");
    return;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    __asm__ volatile ("mv a3, a2");
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_CREATE));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

int thread_create_noStart(thread_t *handle, void (*start_routine)(void *), void *arg) {
    __asm__ volatile ("mv a3, a2");
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_CREATE_NO_START));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

char getc() {
    __asm__ volatile ("mv a0, %0" : : "r" (GETC));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (char)ret;
}

void putc(char) {
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (PUTC));
    __asm__ volatile ("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    __asm__ volatile ("mv a3, a2");
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_OPEN));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

int sem_close(sem_t handle) {
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_CLOSE));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

int sem_wait(sem_t id) {
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_WAIT));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

int sem_signal(sem_t id) {
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_SIGNAL));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

int time_sleep(time_t) {
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r" (TIME_SLEEP));
    __asm__ volatile ("ecall");
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return (ret > 0 ? 0 : -2);
}

void nonKernelMode() {
    __asm__ volatile ("mv a0, %0" : : "r" (NO_KERNEL_MODE));
    __asm__ volatile ("ecall");
}