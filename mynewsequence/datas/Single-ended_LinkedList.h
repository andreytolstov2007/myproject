#ifndef Single_ended_LinkedList_H
#define Single_ended_LinkedList_H

#include "helpfiles/Errors.h"
#include "helpfiles/IEnumerator.h"

template <typename T>
class SLinkedList : public IEnumerable<T> {
private:
    struct Node {
        T data;
        Node *next;
    };

    Node *top;
    Node *low;
    long list_length;

public:
    SLinkedList() {
        top = nullptr;
        low = nullptr;
        list_length = 0;
    }

    SLinkedList(T *items, long length) : SLinkedList() { // опять-таки вопрос. Нужна ли длина?
        if (items == nullptr) { throw NullPointerError(); }
        if (length < 0) { throw IndexOutOfRange(); }

        for (long items_index = 0; items_index < length; items_index ++) {
            Append(items[items_index]);
        }
    }

    SLinkedList(const SLinkedList<T> &other) : SLinkedList() { // для интереса тут генератор. Стоп. Я понял. Не нужно нарушать принцип инкопсуляции. Я же просил нейронку это делать.
        Enumerator<T> enumerator = other.Get_enumerator();
        while (enumerator.Move_next()) {
            Prepend(enumerator.Get_current());
        }
    }

    SLinkedList<T>& operator=(const SLinkedList<T> &other) {
        if (this != &other) {
            Node *current_element;
            for (long counter = 0; counter < list_length; counter ++) {
                current_element = top;
                top = top -> next;
                delete current_element;
            }
            low = nullptr;
            list_length = 0;

            Enumerator<T> enumerator = other.Get_enumerator();
            while (enumerator.Move_next()) {
                Prepend(enumerator.Get_current());
            }
        }
        return *this;
    }

    ~SLinkedList() {
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
        new_node -> next = top;
        top = new_node;
        if (list_length == 0) {
            low = new_node;
        }
        list_length ++;
    }
    
    void Prepend(T item) {
        Node *new_node = new Node();
        new_node -> data = item;
        new_node -> next = nullptr;
        if (list_length == 0) {
            top = new_node;
        }
        else {
            low -> next = new_node;
        }
        low = new_node;
        list_length ++;
    }

    void Insert_at(T item, long list_index) {
        if ((list_index < 0) || (list_index > list_length)) { throw IndexOutOfRange(); }

        if (list_index == 0) {
            Prepend(item);
        }
        else if (list_index == list_length) {
            Append(item);
        }
        else {
            Node *current_element = top;
            for (long counter = 0; counter < (list_length - 1 - list_index); counter ++) {
                current_element = current_element -> next;
            }

            Node *new_node = new Node();
            new_node -> data = item;
            new_node -> next = current_element -> next;
            current_element -> next = new_node;
            list_length ++;
        }
    }

    void Concat(const SLinkedList<T> &other_list) {
        Node *current_element = other_list.top;
        while (current_element != nullptr) {
            Append(current_element -> data);
            current_element = current_element -> next;
        }
    }

    T Pop_last() {
        if (list_length == 0) { throw EmptyCollection(); }

        if (list_length == 1) { low = nullptr; }

        Node *to_delete = top;
        top = to_delete -> next;
        T value = to_delete -> data;
        list_length --;
        delete to_delete;
        return value;
    }

    T Pop_first() {
        if (list_length == 0) { throw EmptyCollection(); }

        T value = low -> data;
        if (list_length == 1) {
            top = nullptr;
            delete low;
            low = nullptr;
        }
        else {
            Node *current_element = top;
            for (long counter = 0; counter < (list_length - 2); counter ++) {
                current_element = current_element -> next;
            }
            delete low;
            low = current_element;
            low -> next = nullptr;
        }
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
            Node *current_element = top;
            for (long counter = 0; counter < (list_length - index - 2); counter ++) {
                current_element = current_element -> next;
            }
            T value = current_element -> next -> data;

            Node *to_delete = current_element -> next;
            current_element -> next = to_delete -> next;
            delete current_element -> next;
            list_length --;
            return value;
        }
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

        Node *current_element = top;
        for (long counter = 0; counter < (list_length - 1 - index); counter ++) {
            current_element = current_element -> next;
        }
        return current_element -> data;
    }

    SLinkedList<T> Get_sub_list(long start_index, long end_index) const {
        if ((start_index < 0) || (end_index >= list_length)) { throw IndexOutOfRange(); }
        if (start_index > end_index) { throw IndexesTranslated(); }
        
        SLinkedList<T> sub_list;
        Node *current_element = top;
        for (long counter = 0; counter < (list_length - 1 - end_index); counter ++) {
            current_element = current_element -> next;
        }
        for (long counter = start_index; counter <= end_index; counter ++) {
            sub_list.Prepend(current_element -> data);
            current_element = current_element -> next;
        }
        return sub_list;
    }

    Enumerator<T> Get_enumerator() const override {
        class SLinkedListEnumerator : public IEnumerator<T> {
        private:
            const SLinkedList<T> *list;
            Node *current_node;
            
        public:
            SLinkedListEnumerator(const SLinkedList<T> *list2) {
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
            
            T Get_current() override { return current_node -> data; }
            
            void Reset() override {
                position = -1;
                current_node = nullptr;
            }
        };
        return Enumerator<T>(new SLinkedListEnumerator(this));
    }
};
#endif //Single_ended_LinkedList.h
