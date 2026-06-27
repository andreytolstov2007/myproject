#ifndef IEnumerator_H
#define IEnumerator_H

template <typename T>
class IEnumerator {
public:
    virtual ~IEnumerator() {}
    virtual bool Move_next() = 0;
    virtual T Get_current() = 0;
    virtual void Reset() = 0;
};

template <typename T>
class Enumerator : public IEnumerator<T> {
private:
    IEnumerator<T> *wrapped;

public:
    explicit Enumerator(IEnumerator<T> *point) : wrapped(point) {}
    virtual ~Enumerator() { delete wrapped; }
    bool Move_next() override { return wrapped -> Move_next(); }
    T Get_current() override { return wrapped -> Get_current(); }
    void Reset() override { wrapped -> Reset(); }
};

template <typename T>
class IEnumerable {
public:
    virtual ~IEnumerable() {}
    virtual Enumerator<T> Get_enumerator() const = 0;
};


template <typename T>
class IBackEnumerator : public IEnumerator<T> {
public:
    virtual bool Move_prev() = 0;
};

template <typename T>
class BackEnumerator : public IBackEnumerator<T> {
private:
    IBackEnumerator<T> *wrapped;

public:
    explicit BackEnumerator(IBackEnumerator<T> *point) : wrapped(point) {}
    virtual ~BackEnumerator() { delete wrapped; }
    bool Move_next() override { return wrapped -> Move_next(); }
    bool Move_prev() override { return wrapped -> Move_prev(); }
    T Get_current() override { return wrapped -> Get_current(); }
    void Reset() override { wrapped -> Reset(); }
};

template <typename T>
class IBackEnumerable {
public:
    virtual ~IBackEnumerable() {}
    virtual BackEnumerator<T> Get_enumerator() const = 0;
};
#endif //IEnumerator_H