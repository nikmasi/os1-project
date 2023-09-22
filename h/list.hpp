//
// Created by os on 8/4/23.
//

#ifndef PROJECT_BASE_LIST_HPP
#define PROJECT_BASE_LIST_HPP

#include "../h/print.hpp"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"

class _thread;
class List {
private:
    struct Elem {
        _thread *data;
        Elem *next;

        Elem(_thread *data, Elem *next) : data(data), next(next) {}
    };
    Elem *head, *tail;

public:
    List(): head(nullptr),tail(nullptr){}
    List(const List &) = delete;
    List &operator=(const List &) = delete;

    void addFirst(_thread *data);
    void addLast(_thread *data);

    _thread *removeFirst();
    _thread *removeLast();

    _thread *peekFirst();
    _thread *peekLast();

    void printList();
    int lengthList();

    void init();

};
#endif //PROJECT_BASE_LIST_HPP
