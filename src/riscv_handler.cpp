//
// Created by os on 8/21/23.
//

#include "../h/riscv.hpp"
#include "../h/print.hpp"

void Riscv::printHandler() {
    printstring("\n sepc: ");
    printinteger(r_sepc());
    printstring("  0x");
    printhexadecimal(r_sepc());
    printstring("\n stval: ");
    printinteger(r_stval());
    printstring("\n");
    printstring("-------------------------\n");
}

void Riscv::otherCase(uint8 oc) {
    if(once)return;
    printstring("\n-------------------------\n");

    if(oc==1)printstring(" Illegal instruction (2)\n");
    else if(oc==2)printstring(" Load access fault (5)\n");
    else if(oc==3)printstring(" Store acess fault (7)\n");
    else{
        printstring("else grana u prekidnoj rutini\n");
        printstring("neocekivan ulazak\n");
    }

    Riscv::printHandler();
    once = true;
}