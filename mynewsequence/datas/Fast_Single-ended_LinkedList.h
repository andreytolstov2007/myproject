#ifndef Fast_Single_ended_LinkedList_H
#define Fast_Single_ended_LinkedList_H

#include "DynamicArray.h"
#include "helpfiles/IEnumerator.h"
#include <iostream>

#define JUMP 4

template <typename T>
class FastSLinkedList {
private:
    struct Node {
        T data;
        DynamicArray<Node*> next{1};
    };

    Node *top;
    Node *low;
    DynamicArray<Node*> boosters;
    long list_length;

public:
    FastSLinkedList () : boosters(4) {
        top = nullptr;
        low = nullptr;
        list_length = 0;
        std::cout << boosters.Get_length() << std::endl;
        std::cout << boosters.Get_allocated_length() << std::endl;
        
    }
};
#endif // Fast_Single_ended_LinkedList_H
