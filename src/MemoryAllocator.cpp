//
// Created by os on 8/6/23.
//
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

FreeMem* MemoryAllocator::free_mem_head= nullptr;
PCBMem* MemoryAllocator::pcb_mem_head= nullptr;

void MemoryAllocator::initMemorySpace() {
    free_mem_head=(FreeMem*)HEAP_START_ADDR;
    free_mem_head->next= nullptr;
    free_mem_head->prev= nullptr;
    free_mem_head->size=(size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR; //- sizeof(FreeMem);
}

void *MemoryAllocator::mem_alloc(size_t size) {
    //size=size/MEM_BLOCK_SIZE;

    if(size<=0)return nullptr;
    if(free_mem_head== nullptr)return nullptr;

    FreeMem* cur=nullptr;
    for(cur=free_mem_head;cur!=0;cur=cur->next){

        if(cur->size< ((size)))continue;

        if(cur->size -(size)<= sizeof(FreeMem)){
            if(cur->prev)cur->prev->next=cur->next;
            else free_mem_head=cur->next;
            if(cur->next)cur->next->prev=cur->prev;
        }
        else{
            FreeMem* newfrgm =(FreeMem*)((char*)cur+(size*MEM_BLOCK_SIZE));
            if(cur->prev)cur->prev->next=newfrgm;
            else free_mem_head=newfrgm;
            if(cur->next)cur->next->prev=newfrgm;

            newfrgm->prev=cur->prev;
            newfrgm->next=cur->next;
            newfrgm->size=cur->size-(size*MEM_BLOCK_SIZE);

        }

        //adding to PCBMem

        PCBMem* tek= nullptr;
        if(!pcb_mem_head || (char*)cur< (char*)pcb_mem_head) tek= nullptr;
        else{
            for(tek=pcb_mem_head;tek->next!= nullptr && (char*)cur > (char*)(tek->next);tek=tek->next);
        }

        PCBMem* newSeg=(PCBMem*)cur;
        newSeg->size=(size);
        newSeg->prev=tek;
        if(tek)newSeg->next=tek->next;
        else newSeg->next=pcb_mem_head;

        if(newSeg->next)newSeg->next->prev=newSeg;
        if(tek)tek->next=newSeg;
        else pcb_mem_head=newSeg;
        return (void*)((char*)cur+ sizeof(PCBMem));//mozda ne +sizeof(PCBMem);
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *address) {
    if(address==nullptr)return -1;
    if((char*)address<(char*)HEAP_START_ADDR || (char*)address>(char*)HEAP_END_ADDR)return -2;


    //to find in PCBMem
    if(pcb_mem_head==nullptr)return -3;
    PCBMem* pom=pcb_mem_head;
    PCBMem* segDeal=(PCBMem*)((char*)address-sizeof(PCBMem));

    bool found=false;
    for(pom=pcb_mem_head;pom && pom<=segDeal;pom=pom->next){
        if(pom==segDeal){
            if (pom->prev) pom->prev->next = pom->next;
            else pcb_mem_head = pom->next;
            if (pom->next) pom->next->prev = pom->prev;
            found=true;
        }
    }
    if(!found)return -4;

    //in FreeMem
    FreeMem* cur=nullptr;
    if(!free_mem_head || (char*)address<(char*)free_mem_head)cur= nullptr;
    else{
        for(cur=free_mem_head;cur->next!= nullptr && (char*)address>(char*)(cur->next);cur=cur->next);
    }

    FreeMem* newSeg=(FreeMem*)((char*)address-sizeof(PCBMem));
    if(newSeg==nullptr)return -5;

    if(pom!= nullptr)newSeg->size=pom->size;
    newSeg->prev=cur;
    if(cur)newSeg->next=cur->next;
    else newSeg->next=free_mem_head;
    if(newSeg->next)newSeg->next->prev=newSeg;
    if(cur)cur->next=newSeg;
    else free_mem_head=newSeg;
    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}

int MemoryAllocator::tryToJoin(FreeMem *cur) {
    if(!cur)return 0;
    if(cur->next && (char*)cur+cur->size ==(char*)(cur->next)){
        cur->size+=cur->next->size;
        cur->next=cur->next->next;

        //if(cur->next)cur->next->prev=cur; //?

        return 1;
    }else
        return 0;
}
