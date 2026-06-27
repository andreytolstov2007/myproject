#ifndef DynamicArray_H
#define DynamicArray_H

#include "helpfiles/option.h"

template <typename T>
class DynamicArray {
private:
    T *arr;
    long array_length;
    long size;

public:
    DynamicArray() {
        array_length = 16;
        size = 0;
        arr = new T[array_length];
    }

    explicit DynamicArray(long len) {
        if (len < 0) { throw IndexOutOfRange(); }

        array_length = len;
        size = 0;
        arr = new T[array_length]; // нужно поменять. вдруг тип не имеет конструктор по умолчанию
    }

    DynamicArray(T *items, long length) {  // нужна ли от пользователя длина? может это тот случай, где имеет смысл sizeof(items)/sizeof(T)
        if (items == nullptr) { throw NullPointerError(); }
        if (length < 0) { throw IndexOutOfRange(); }

        arr = new T[length];
        array_length = length;
        size = length;
        for (long arr_index = 0; arr_index < length; arr_index ++) {
            arr[arr_index] = items[arr_index];
        }
    }

    DynamicArray(const DynamicArray<T> &other) {
        this -> size = other.Get_length();
        this -> array_length = other.array_length;
        this -> arr = new T[array_length];
        for (long arr_index = 0; arr_index < size; arr_index ++) {
            this -> arr[arr_index] = other.Get(arr_index);
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T> &other) {
        if (this != &other) {
            delete []arr;
            size = other.Get_length();
            array_length = other.array_length;
            arr = new T[array_length];
            for (long arr_index = 0; arr_index < size; arr_index ++) {
                arr[arr_index] = other.Get(arr_index);
            }
        }
        return *this;
    }

    ~DynamicArray() { delete []arr; }

    void Resize(long new_length) {
        if (new_length < 0) { throw IndexOutOfRange(); }

        T *new_arr = new T[new_length];
        array_length = new_length;
        if (new_length < size) {
            size = new_length;
        }
        for (long new_arr_index = 0; new_arr_index < size; new_arr_index ++) {
            new_arr[new_arr_index] = arr[new_arr_index];
        }
        delete []arr;
        arr = new_arr;
    }

    T Get(long index) const {
        if ((index < 0) || (index >= size)) { throw IndexOutOfRange(); }
        return arr[index];
    }

    long Get_length() const { return size; }

    long Get_allocated_length() const { return array_length; }

    void Set(long index, T value) {
        if ((index < 0) || (index >= size)) { throw IndexOutOfRange(); }
        arr[index] = value;
    }

    void Zero() {
        for (long arr_index = 0; arr_index < array_length; arr_index ++) {
            arr[arr_index] = T();
        }
        size = array_length;
    }

    void Append(T item) {
        if (size == array_length) {
            if (array_length == 0) {
                Resize(1);
            }
            else {
                Resize(array_length * 2);
            }
        }
        arr[size] = item;
        size ++;
    }

    void Prepend(T item) {
        if (size == array_length) {
            if (array_length == 0) {
                Resize(1);
            }
            else {
                Resize(array_length * 2);
            }
        }
        for (long arr_index = size; arr_index > 0; arr_index --) {
            arr[arr_index] = arr[arr_index - 1];
        }
        arr[0] = item;
        size ++;
    }

    void Insert_at(T item, long index) {
        if ((index < 0) || (index > size)) { throw IndexOutOfRange(); }

        if (size == array_length) {
            if (array_length == 0) {
                Resize(1);
            }
            else {
                Resize(array_length * 2);
            }
        }
        for (long arr_index = size; arr_index > index; arr_index --) {
            arr[arr_index] = arr[arr_index - 1];
        }
        arr[index] = item;
        size ++;
    }

    T Pop(long index) {
        if (index == 0) { throw EmptyCollection(); }
        if ((index < 0) || (index > size)) { throw IndexOutOfRange(); }

        T value = arr[index];
        for (long arr_index = index; arr_index < (size - 1); arr_index ++) {
            arr[arr_index] = arr[arr_index + 1];
        }
        size --;
        return value;
    }

    Option<long> last_remove(T value) {
        long arr_index = size - 1;
        while ((arr_index != -1) && (arr[arr_index] != value)) {
            arr_index --;
        }

        if (arr_index == -1) {
            return Option<long>();
        }
        else {
            Pop(arr_index);
            return Option<long>(arr_index);
        }
    }

    Option<long> first_remove(T value) {
        long arr_index = 0;
        while ((arr_index != size) && (arr[arr_index] != value)) {
            arr_index ++;
        }

        if (arr_index == size) {
            return Option<long>();
        }
        else {
            Pop(arr_index);
            return Option<long>(arr_index);
        }
    }

    DynamicArray<T> Get_sub_list(long start_index, long end_index) {
        if ((start_index < 0) || (end_index >= list_length)) { throw IndexOutOfRange(); }
        if (start_index > end_index) { throw IndexesTranslated(); }
        
        DynamicArray<T> new_list(end_index - start_index + 1);
        for (long arr_index = start_index; arr_index <= end_index; arr_index ++) {
            new_list.Append(arr[arr_index]);
        }
        return new_list;
    }
};
#endif //DynamicArray_H