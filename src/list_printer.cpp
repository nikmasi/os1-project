//
// Created by os on 8/24/23.
//
#include "../h/list.hpp"

void List::printList(){
    Elem* current = head;

    printstring("Head of the list: 0x");

    printhexadecimal((uint64)head);
    printstring("\n");
    while (current) {
        _thread* data = current->data;

        printstring("0x");
        printhexadecimal((uint64) (data));
        printstring("\n");
        current = current->next;
    }
}

void List::init() {
    this->head = 0;
    this->tail = 0;
}

