#ifndef Double_ended_LinkedList_H
#define Double_ended_LinkedList_H

#include "helpfiles/Errors.h"
#include "helpfiles/IEnumerator.h"

template <typename T>
class DLinkedList : public IBackEnumerable<T> {
    private:
    struct Node {
        T data;
        Node *next;
        Node *prev;
    };

    Node *top;
    Node *low;
    long list_length;

    Node* Find_node_by_index(long index) const {
        Node *current_element;
        if (index < (list_length / 2)) {
            current_element = top;
            for (long counter = 0; counter < index; counter ++) {
                current_element = current_element -> next;
            }
        }
        else {
            current_element = low;
            for (long counter = (list_length - 1); counter > index; counter --) {
                current_element = current_element -> prev;
            }
        }
        return current_element;
    }

public:
    DLinkedList() {
        top = nullptr;
        low = nullptr;
        list_length = 0;
    }

    DLinkedList(T *items, long length) : DLinkedList() { // опять-таки вопрос. Нужна ли длина?
        if (items == nullptr) { throw NullPointerError(); }
        if (length < 0) { throw IndexOutOfRange(); }

        for (long items_index = 0; items_index < length; items_index ++) {
            Append(items[items_index]);
        }
    }

    DLinkedList(const DLinkedList<T> &other) : DLinkedList() {
        BackEnumerator<T> enumerator = other.Get_enumerator();
        while (enumerator.Move_next()) {
            Append(enumerator.Get_current());
        }
    }

    DLinkedList<T>& operator=(const DLinkedList<T> &other) {
        if (this != &other) {
            Node *current_element;
            for (long counter = 0; counter < list_length; counter ++) {
                current_element = top;
                top = top -> next;
                delete current_element;
            }
            low = nullptr;
            list_length = 0;

            BackEnumerator<T> enumerator = other.Get_enumerator();
            while (enumerator.Move_next()) {
                Append(enumerator.Get_current());
            }
        }
        return *this;
    }

    ~DLinkedList() {
        Node *current_element;
        for (long counter = 0; counter < list_length; counter ++) {
            current_element = top;
            top = current_element -> next;
            delete current_element;
        }
    }
    
    void Append(T item) {
        Node *new_node = new Node();
        new_node -> data = item;
        new_node -> next = nullptr;
        new_node -> prev = low;

        if (list_length == 0) {
            top = new_node;
        }
        else {
            low -> next = new_node;
        }
        low = new_node;
        list_length ++;
    }
    
    void Prepend(T item) {
        Node *new_node = new Node();
        new_node -> data = item;
        new_node -> next = top;
        new_node -> prev = nullptr;

        if (list_length == 0) {
            top = new_node;
            low = new_node;
        }
        else {
            top -> prev = new_node;
            top = new_node;
        }
        list_length ++;
    }

    void Insert_at(T item, long index) {
        if ((index < 0) || (index > list_length)) { throw IndexOutOfRange(); }

        if (index == 0) {
            Prepend(item);
        }
        else if (index == list_length) {
            Append(item);
        }
        else {
            Node *current_element = Find_node_by_index(index);

            Node *new_node = new Node();
            new_node -> data = item;
            new_node -> prev = current_element;
            new_node -> next = current_element -> next;
            current_element -> next -> prev = new_node;
            current_element -> next = new_node;
            list_length ++;
        }
    }

    void Concat(const DLinkedList<T> &other_list) const {
        BackEnumerator<T> enumerator = other_list.Get_enumerator();
        while (enumerator.Move_next()) {
            Append(enumerator.Get_current());
        }
    }

    T Pop_first() {
        if (list_length == 0) { throw EmptyCollection(); }

        Node *to_delete = top;
        T value = to_delete -> data;

        if (list_length == 1) {
            top = nullptr;
            low = nullptr;
        }
        else {
            top = to_delete -> next;
            top -> prev = nullptr;
        }
        delete to_delete;
        list_length --;
        return value;
    }

    T Pop_last() {
        if (list_length == 0) { throw EmptyCollection(); }

        Node *to_delete = low;
        T value = to_delete -> data;

        if (list_length == 1) {
            top = nullptr;
            low = nullptr;
        }
        else {
            low = to_delete -> prev;
            low -> next = nullptr;
        }
        delete to_delete;
        list_length --;
        return value;
    }

    T Pop(long index) {
        if (list_length == 0) { throw EmptyCollection(); }
        if ((index < 0) || (index >= list_length)) { throw IndexOutOfRange(); }

        if (index == 0) {
            return Pop_first();
        }
        else if (index == (list_length - 1)) {
            return Pop_last();
        }
        else {
            Node *to_delete = Find_node_by_index(index);
            T value = current_element -> data;
            to_delete -> next -> prev = to_delete -> prev;
            to_delete -> prev -> next = to_delete -> next;
            delete to_delete;
            list_length --;
            return value;
        }
    }

    T Get_first() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return top -> data;
    }

    T Get_last() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return low -> data;
    }

    long Get_length() const { return list_length; }

    T Get_by_index(long index) const {
        if ((index < 0) || (index >= list_length)) { throw IndexOutOfRange(); }

        Node *current_element = Find_node_by_index(index);
        return current_element -> data;
    }

    /*T& operator[](long list_index) {
        if ((list_index < 0) || (list_index >= list_length)) { throw IndexOutOfRange(); }

        Node *current_element = Find_node_by_index(list_index);
        return current_element -> data;
    }

    const T& operator[](long index) const {
        if ((index < 0) || (index >= list_length)) { throw IndexOutOfRange(); }
        
        Node *current_element = Find_node_by_index(index);
        return current_element -> data;
    }*/

    DLinkedList<T> Get_sub_list(long start_index, long end_index) const {
        if ((start_index < 0) || (end_index >= list_length)) { throw IndexOutOfRange(); }
        if (start_index > end_index) { throw IndexesTranslated(); }

        DLinkedList<T> sub_list;
        if (start_index < (list_length - 1 - end_index)) {
            Node *current_element = top;
            for (long counter = 0; counter < start_endex; counter ++) {
                current_element = current_element -> next;
            }
            for (long counter = start_index; counter <= end_index; counter ++) {
                sub_list.Append(current_element -> data);
                current_element = current_element -> next;
            }
        }
        else {
            Node *current_element = low;
            for (long counter = (list_length - 1); counter > end_endex; counter --) {
                current_element = current_element -> prev;
            }
            for (long counter = end_index; counter >= start_index; counter --) {
                sub_list.Prepend(current_element -> data);
                current_element = current_element -> prev;
            }
        }
        return sub_list;
    }

    BackEnumerator<T> Get_enumerator() const override{
        class DLinkedListEnumerator : public IBackEnumerator<T> {
        private:
            const DLinkedList<T> *list;
            Node *current_node;
            
        public:
            DLinkedListEnumerator(const DLinkedList<T> *list2) {
                list = list2;
                current_node = nullptr;
            }
            
            bool Move_next() override {
                if (current_node == nullptr) {
                    current_node = list -> top;
                    return current_node != nullptr;
                }
                current_node = current_node -> next;
                return current_node != nullptr;
            }

            bool Move_prev() override {
                if (current_node == nullptr) {
                    current_node = list -> low;
                    return current_node != nullptr;
                }
                current_node = current_node -> prev;
                return current_node != nullptr;
            }
            
            T Get_current() override { return current_node -> data; }
            
            void Reset() override { current_node = nullptr; }
        };
        return BackEnumerator<T>(new DLinkedListEnumerator(this));
    }
};
#endif // Double_ended_LinkedList_H