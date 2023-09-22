//
// Created by os on 8/3/23.
//
#include "../h/riscv.hpp"
#include "../h/_thread.hpp"

bool Riscv::once=false;

void Riscv::popSppSpie()
{
    if(_thread::running==_thread::main)ms_sstatus(SSTATUS_SPP);
    else mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}