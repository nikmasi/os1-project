//
// Created by os on 8/4/23.
//
#include "../h/_thread.hpp"
#include "../h/scheduler.hpp"


bool checkIsMain(){
    if(_thread::running->getId()==0){
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
        return true;
    }else if(_thread::running->getId()==1){
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
        return false;
    }else{
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
        return false;
    }
}

uint64 _thread::timeSliceCounter = 0;
_thread* _thread::main = nullptr;
_thread* _thread::idle= nullptr;
bool _thread::finished = false;
_thread *_thread::running = nullptr;
uint64 _thread::sId=-1;

bool _thread::isStateFinished() { return running->thread_state==FINISHED;}
void _thread::setStateFinished() { running->thread_state=FINISHED;}

void _thread::setState(ThreadState state) {
    //CREATED, READY, RUNNING, BLOCKED, FINISHED,SLEEPING
    if(state==CREATED){ this->thread_state=CREATED; }
    else if(state==READY){this->thread_state=READY;}
    else if(state==RUNNING){this->thread_state=RUNNING;}
    else if(state==BLOCKED){this->thread_state=BLOCKED;}
    else if(state==FINISHED){this->thread_state=FINISHED;}
    else if(state==SLEEPING){this->thread_state=SLEEPING;}
}

uint64 _thread::getTimeSliceCounter() {return timeSliceCounter; }

void _thread::setTimeSliceCounter(uint64 timeSliceCounter) {_thread::timeSliceCounter=timeSliceCounter;}

_thread *_thread::initMain() {
    if(!main){
        main = new _thread(nullptr, nullptr);
        main->thread_state=RUNNING;
        running=main;
    }
    return main;
}

_thread *_thread::initIdle() {
    if(!idle){
        idle=new _thread(idleJob, nullptr);
        idle->thread_state=CREATED;
        Scheduler::put(idle);
    }
    return idle;
}

void _thread::dispatch() {

    _thread *old=running;

    if(old->thread_state!=FINISHED && old->thread_state!=BLOCKED && old->thread_state!=SLEEPING ){
        Scheduler::put(old);
        old->thread_state=READY;
    }
    running=Scheduler::get();
    if(running==idle){
        Scheduler::put(running);
        running=Scheduler::get();
    }
    while (running && running->thread_state == FINISHED){
        running = Scheduler::get();
    }

    if(running)running->setState(RUNNING);

    if (_thread::running == nullptr) {
        running = main;
        //Scheduler::printScheduler();
    }
    checkIsMain();

    _thread::contextSwitch(&old->context,&running->context);
}

void _thread::threadWrapper() {
    //Riscv::popSppSpie();
    running->thread_state= RUNNING;
    running->body(running->arg);
    exit();
}

void _thread::yield() {
    __asm__ volatile ("mv a0, %0" : : "r" (0x13));
    __asm__ volatile ("ecall");
}

int _thread::start() {
    if(this->thread_state != CREATED) return -1;
    this->thread_state = READY;
    if (this->id != 0 && this!=main && this->id!=1 && this!=idle) { Scheduler::put(this); }
    return 0;
}

_thread *_thread::createThread(_thread::Body body, void *arg) {
    return new _thread(body, arg);
}

_thread *_thread::threadInit(_thread::Body body, void *arg) {
    _thread* current = (_thread*)MemoryAllocator::mem_alloc(sizeof(_thread));
    uint64* stack = (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
    current->arg = arg;
    current ->body = body;
    current ->stack = body != nullptr ? stack : nullptr;
    current -> context = {body != nullptr ? (uint64) &threadWrapper: 0,
                          stack != nullptr ? (uint64) &(current->stack[STACK_SIZE]) : 0};
    current->thread_state = CREATED;
    current->timeSlice = DEFAULT_TIME_SLICE;
    current->id=++sId;
    return current;
}

int _thread::exit() {
    if(running->thread_state!=FINISHED){
        running->thread_state=FINISHED;
        dispatch();
        return 0;
    }
    return -1;
}

void *_thread::operator new(size_t size) {
     return MemoryAllocator::mem_alloc(size);
}

void *_thread::operator new[](size_t size) {
    return MemoryAllocator::mem_alloc(sizeof(_thread));
}

_thread *_thread::createThread(_thread::Body body, void *arg, uint64 *stack) {
    _thread* t =new _thread(body, arg, stack, DEFAULT_TIME_SLICE);
    t->start();
    return t;
}

void _thread::idleJob(void *) {
    while(true){
        thread_dispatch();
    }
}

void _thread::join(_thread **handle) {
    while(!(*handle)->isStateFinished())_thread::dispatch();
}