//
// Created by os on 8/5/23.
//
#include "../h/scheduler.hpp"

List Scheduler::readyCoroutineQueue;

_thread *Scheduler::get(){
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(_thread *ccb){
    readyCoroutineQueue.addLast(ccb);
}

void Scheduler::printScheduler() {
    readyCoroutineQueue.printList();
}