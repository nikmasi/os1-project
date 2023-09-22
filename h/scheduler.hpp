//
// Created by os on 8/5/23.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP
#include "list.hpp"

class Scheduler
{
//private:
  //  static List readyCoroutineQueue;
public:
    static _thread *get();
    static void put(_thread *ccb);

    static void printScheduler();

    static List readyCoroutineQueue; // u private, zbog debuggera ovde
};
#endif //PROJECT_BASE_SCHEDULER_HPP
