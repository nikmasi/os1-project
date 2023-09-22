//
// Created by os on 8/4/23.
//
#include "../h/list.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"

void List::addFirst(_thread *data)
{
    //Elem *elem = (Elem*)mem_alloc(sizeof (Elem)) ;
    Elem *elem = (Elem*)__mem_alloc(sizeof (Elem)) ;

    elem->data = data;
    elem->next = head;
    head = elem;
    if (!tail) { tail = head; }
}

void List::addLast(_thread *data)
{
    //Elem *elem = (Elem*)MemoryAllocator::mem_alloc(sizeof (Elem)) ;

    Elem *elem = (Elem*)__mem_alloc(sizeof(Elem)) ;


    elem->data = data;
    elem->next = nullptr;

    if (tail){
        tail->next = elem;
        tail = elem;
    } else{ head = tail = elem; }
}

_thread *List::removeFirst()
{
    if (!head) { return nullptr; }

    Elem *elem = head;
    head = head->next;
    if (!head) { tail = nullptr; }

    _thread *ret = elem->data;

    //mem_free((void*)elem);
    __mem_free(elem);
    delete elem;
    return ret;
}

_thread *List::removeLast()
{
    if (!head) { return nullptr; }

    Elem *prev = 0;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next){ prev = curr; }

    Elem *elem = tail;
    if (prev) { prev->next = 0; }
    else { head = 0; }
    tail = prev;

    _thread *ret = elem->data;


    __mem_free((void*)elem); //delete elem

    return ret;
}

_thread *List::peekFirst()
{
    if (!head) { return 0; }
    return head->data;
}

_thread *List::peekLast()
{
    if (!tail) { return 0; }
    return tail->data;
}

int List::lengthList() {
    Elem* tek = head;
    int cnt=0;
    while(tek){
        cnt++;
        tek=tek->next;
    }
    return cnt;
}