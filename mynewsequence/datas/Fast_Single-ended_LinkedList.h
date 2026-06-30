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
        long nomber;
        DynamicArray<Node*> next{1};
    };

    Node *top;
    Node *low;
    Node *start;
    DynamicArray<Node*> boosters_to;
    DynamicArray<Node*> boosters_from;
    long list_length;

public:
    FastSLinkedList () : boosters_to(4), boosters_from(4) {
        top = nullptr;
        low = nullptr;
        start = nullptr;
        list_length = 0;
    }

    ~FastSLinkedList() {
        Node *current_element = top;
        while (current_element != nullptr) {
            top = current_element -> next.Get(0);
            delete current_element;
            current_element = top;
        }
    }

    void Append(T value) {
        Node *ptr = new Node();
        ptr -> data = value;
        ptr -> next.Append(top);
        if (list_length == 0) {
            low = ptr;
            ptr -> nomber = 0;
            start = ptr;
            boosters_to.Append(ptr);
            boosters_from.Append(ptr);
        }
        else {
            ptr -> nomber = top -> nomber + 1;
        }
        top = ptr;
        list_length ++;

        int count = 0;
        long index = top -> nomber;
        if (index != 0) {
            while (index % JUMP == 0) {
                if (count >= boosters_to.Get_length()) { //слишком много срабатывает if этот. Может можно как-то подругому?
                    boosters_to.Append(start);
                }
                ptr -> next.Append(boosters_to.Get(count));
                boosters_to.Set(count, ptr);
                index /= JUMP;
                count ++;
            }
        }
        /*std::cout << ptr << " | ";
        for (int i = 0; i < ptr -> next.Get_length(); i ++) {
            std::cout << ptr -> next.Get(i) << " ";
        }
        std::cout << std::endl;*/
    }

    void Prepend(T value) {
        Node *ptr = new Node();
        ptr -> data = value;
        ptr -> next.Append(nullptr);
        if (list_length == 0) {
            top = ptr;
            ptr -> nomber = 0;
            start = ptr;
            boosters_to.Append(ptr);
            boosters_from.Append(ptr);
        }
        else {
            low -> next.Set(0, ptr);
            ptr -> nomber = low -> nomber - 1;
        }
        low = ptr;
        list_length ++;

        long index = (-1) * low -> nomber;
        if (index != 0) {
            int count = 0;
            while (index % JUMP == 0) {
                if (count >= boosters_from.Get_length()) { //слишком много срабатывает if этот. Может можно как-то подругому?
                    boosters_from.Append(start);
                }
                boosters_from.Get(count) -> next.Append(ptr);
                boosters_from.Set(count, ptr);
                index /= JUMP;
                count ++;
            }
        }
        /*for (int i = 0; i < ptr -> next.Get_length(); i ++) {
            std::cout << ptr << " | " << ptr -> next.Get(i) << " ";
        }
        std::cout << std::endl;*/
    }

    long Get_length() const { return list_length; }

    T Get_first() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return low -> data;
    }

    T Get_last() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return top -> data;
    }

    T Get_by_index(long index) const {
        if ((index < 0) || (index >= list_length)) { throw IndexOutOfRange(); }

        std::cout << "11111111111111111" << std::endl;

        long need_nomber = index + low -> nomber;
        Node *current_element = top;
        /*if (need_nomber <= 0) {
            current_element = start;
        }*/

        std::cout << current_element -> nomber << std::endl;
        long next_index = 0;
        while (next_index < (current_element -> next.Get_length() - 1)) {
            next_index ++;
            if (current_element -> next.Get(next_index) -> nomber < need_nomber) {
                next_index --;
                break;
            }
        }

        std::cout << "2222222222222222" << std::endl;
        std::cout << next_index << std::endl;
        while (current_element -> next.Get(next_index) -> nomber > need_nomber) {
            current_element = current_element -> next.Get(next_index);
            if (next_index != (current_element -> next.Get_length() - 1)) {
                if (next_index == (current_element -> next.Get_length() - 2)) {
                    next_index ++;
                }
                else {
                    next_index = current_element -> next.Get_length() - 1;
                }
            }
        }

        std::cout << "333333333333333333" << std::endl;
        std::cout << current_element -> nomber << std::endl;
        if (current_element -> nomber == need_nomber) {
            return current_element -> data;
        }
        while (current_element -> next.Get(next_index) -> nomber < need_nomber) {
            next_index --;
        }

        std::cout << "3333333333333333333334444444444444444444444444" << std::endl;
        std::cout << index << " | " << need_nomber << " | " << current_element -> nomber << std::endl;

        while (current_element -> nomber != need_nomber) {
            current_element = current_element -> next.Get(next_index);
            std::cout << "111" << std::endl;
            if (next_index != (current_element -> next.Get_length() - 1)) {
                next_index --;
            }
            else if (current_element -> next.Get(next_index) -> nomber < need_nomber) {
                next_index --;
            }
            std::cout << "333" << std::endl;
        }

        std::cout << "444444444444444444" << std::endl;
        std::cout << current_element -> nomber << std::endl;
        return current_element -> data;
    }

    void printf() const {
        Node *current = top;
        std::cout << list_length << std::endl;
        for (long i = 0; i < list_length; i ++) {
            std::cout << current << " | ";
            for (long j = 0; j < current -> next.Get_length(); j ++) {
                std::cout << current -> next.Get(j) << " ";
            }
            std::cout << current -> nomber;
            std::cout << std::endl;
            current = current -> next.Get(0);
        }
    }
};
#endif // Fast_Single_ended_LinkedList_H
