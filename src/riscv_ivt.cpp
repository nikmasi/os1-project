//
// Created by os on 8/3/23.
//

#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/numCode.hpp"
#include "../lib/mem.h"
#include "../lib/console.h"
#include "../h/_thread.hpp"
#include "../h/scauseValues.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/_sem.hpp"

void Riscv::handleIntr() {
    uint64 code = 0;
    __asm__ volatile ("mv %0, a0":"=r" (code));
    uint64 arrg1, arrg2, arrg3, arrg4;
    __asm__ volatile ("mv %0, a1":"=r" (arrg1));
    __asm__ volatile ("mv %0, a2":"=r" (arrg2));
    __asm__ volatile ("mv %0, a3":"=r" (arrg3));
    __asm__ volatile ("mv %0, a4":"=r" (arrg4));

    uint64 scause = r_scause();

    if (scause == U_MODE || scause == S_MODE){
        Riscv::mc_sstatus(1<<1);
        uint64 sepc = r_sepc();
        sepc=sepc+4;
        uint64 sstatus = r_sstatus();
        switch (code) {
            case MEM_ALLOC:{
                uint64 *ret = (uint64*)MemoryAllocator::mem_alloc(arrg1);
                __asm__ volatile("sd a0, 10*8(fp)");
                __asm__ volatile ("mv a0, %[addr]" : : [addr]"r"(ret));
                break;
            }
            case MEM_FREE:{
                void *argT = (void *)arrg1;
                uint ret = MemoryAllocator::mem_free(argT);
                __asm__ volatile("sd a0, 10*8(fp)");
                __asm__ volatile ("mv a0, %[addrt]" : : [addrt]"r"(ret));
                break;
            }
            case THREAD_CREATE:{
                //printstring("thread create\n");
                thread_t *handle;
                handle = (thread_t*)arrg1;
                _thread::Body body = (_thread::Body)arrg2;
                void* arg = (void*)arrg3;
                *handle = _thread::threadInit(body, arg);
                //*handle= _thread::createThread(body, arg, (uint64*)arrg1);

                (*handle)->start();
                __asm__ volatile("mv a0, %0" : :"r"(handle));
                __asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case THREAD_CREATE_NO_START:{
                thread_t *handle;
                handle = (thread_t*)arrg1;
                _thread::Body body = (_thread::Body)arrg2;
                void* arg = (void*)arrg3;
                *handle = _thread::threadInit(body, arg);
                __asm__ volatile("mv a0, %0" : :"r"(handle));
                __asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case THREAD_EXIT:{
                _thread *handle = (_thread*)arrg1;
                handle->exit();
                break;
            }
            case THREAD_DISPATCH:{
                _thread::dispatch();
                break;
            }
            case THREAD_JOIN:{
                thread_t *handle = (thread_t*)arrg1;
                _thread::join(handle);
                break;
            }
            case NO_KERNEL_MODE:{
                w_sstatus(sstatus);
                mc_sstatus(1<<8);
                __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
                mc_sip(1<<1);
                return;
                break;
            }
            case SEM_OPEN:{
                sem_t* h=(sem_t*)arrg1;
                uint64 init=(uint64)arrg2;
                _sem::createSemaphore(h, init);
                __asm__ volatile("mv a0, %0" : :"r"(0));
                //__asm__ volatile("sd a0, 10*8(fp)");
                break;
            }
            case SEM_CLOSE:{
                //printstring("sem close\n");
                //sem_t* h=(sem_t*)arrg1;
                sem_t handle=(sem_t)arrg1;
                handle->close();
                break;
            }
            case SEM_WAIT:{
                //printstring("sem wait\n");
                sem_t handle = (sem_t)arrg1;
                handle->wait();
                uint64 ret;
                if (handle== nullptr) ret = -1;
                __asm__ volatile("mv a0, %0" : :"r"(ret));
                break;
            }
            case SEM_SIGNAL:{
                //printstring("sem_signal\n");
                sem_t handle = (sem_t)arrg1;
                handle->signal();
                uint64 ret;
                if (handle== nullptr) ret = -1;
                __asm__ volatile("mv a0, %0" : :"r"(ret));
                break;
            }
            case TIME_SLEEP:{
                break;
            }
            case PUTC:{
                char c = (char)arrg1;
                __putc(c);
                break;
            }
            case GETC:{
                uint64 c = __getc();
                //printstring("Char :  ");printinteger(c);
                __asm__ volatile("sd a0, 10*8(fp)");
                __asm__ volatile("mv a0, %0" : : "r"(c));
                break;
            }
        }
        w_sepc(sepc);
        Riscv::mc_sstatus(1<<1);
        w_sstatus(sstatus);
    }
    else if(scause==ILLEGAL_INSTRUCTION){otherCase(1);return;}
    else if(scause==LOAD_ACCESS_FAULT){otherCase(2);return;}
    else if(scause==STORE_ACCESS_FAULT){otherCase(3);return;}
    else{otherCase(4);return;}
    return;
}

void Riscv::handleTime() {
    uint64 code = 0;
    __asm__ volatile ("mv %0, a0":"=r" (code));
    uint64 arrg1, arrg2, arrg3, arrg4;
    __asm__ volatile ("mv %0, a1":"=r" (arrg1));
    __asm__ volatile ("mv %0, a2":"=r" (arrg2));
    __asm__ volatile ("mv %0, a3":"=r" (arrg3));
    __asm__ volatile ("mv %0, a4":"=r" (arrg4));

    uint64 scause = r_scause();

    if (scause == S_SOFTWARE_INTERRUPT){mc_sip(1<<1);}
    return;
}

void Riscv::handleConsole() {
    uint64 code = 0;
    __asm__ volatile ("mv %0, a0":"=r" (code));
    uint64 arrg1, arrg2, arrg3, arrg4;
    __asm__ volatile ("mv %0, a1":"=r" (arrg1));
    __asm__ volatile ("mv %0, a2":"=r" (arrg2));
    __asm__ volatile ("mv %0, a3":"=r" (arrg3));
    __asm__ volatile ("mv %0, a4":"=r" (arrg4));

    uint64 scause = r_scause();

    if(scause == S_EXTERNAL_INTERRUPT){console_handler();}
    return;
}