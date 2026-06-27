#ifndef Option_H
#define Option_H

#include "Errors.h"

template <typename T>
class Option {
private:
    bool has_value;
    union { T value; };

    void destroy_value() {
        if (has_value) {
            value.~T();
        }
    }
    
    void copy_value(const T &other) {
        new (&value) T(other);
    }

public:
    Option() : has_value(false) {}
    
    explicit Option(const T &val) : has_value(true) {
        new (&value) T(val);
    }

    Option(const Option &other) : has_value(other.has_value) {
        if (has_value) {
            copy_value(other.value);
        }
    }

    ~Option() { destroy_value(); }

    Option& operator=(const Option &other) {
        if (this != &other) {
            destroy_value();
            has_value = other.has_value;
            if (has_value) {
                copy_value(other.value);
            }
        }
        return *this;
    }

    bool Is_some() const { return has_value; }

    bool Is_none() const { 
        if (has_value == true) {
            return false;
        }
        return true;
    }
    
    T Get() const {
        if (has_value == false) {
            throw EmptyCollection();
        }
        return value;
    }
    
    T Get_or_default(const T &defaultValue) const {
        if (has_value == true) {
            return value;
        } else {
            return defaultValue;
        }
    }
    
    template <typename ResultType>
    Option<ResultType> Map(ResultType (*function)(T)) const {
        if (has_value == true) {
            return Option<ResultType>(function(value));
        } else {
            return Option<ResultType>();
        }
    }
};
#endif //Option_H