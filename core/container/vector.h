#ifndef __CONTAINERS_VECTOR__
#define __CONTAINERS_VECTOR__

#include <string.h>
#include <assert.h>
#include "core/traits.h"
#include "defines.h"
#include "shared_object.h"

namespace BCosta {
namespace Core {
namespace Container {

//!
template<class T>
struct StandardVectorPolicy
{
    static void Init(T *, int)
    { }

    static void Copy(T *, int)
    { }

    static void Destroy(T *, int)
    { }

    static void Push(T)
    { }

    static void Pull(T)
    { }
};

//!
template<class T>
struct AutoVectorPolicy
{
    static void Init(T *data, int size)
    {
        for (int i = 0; i < size; ++i)
            data[i] = 0;
    }

    static void Copy(T *, int)
    { }

    static void Destroy(T *data, int size)
    {
        for (int i = 0; i < size; ++i)
            delete data[i];
    }

    static void Push(T)
    { }

    static void Pull(T p)
    { delete p; }
};

//!
template<class T>
struct SharedVectorPolicy
{
    static void Init(T *data, int size)
    {
        for (int i = 0; i < size; ++i)
            data[i] = 0;
    }

    static void Copy(T *d, int size)
    {
        for (int i = 0; i < size; ++i)
            if (T _d = d[i])
                _d->IncRef();
    }

    static void Destroy(T *data, int size)
    {
        for (int i = 0; i < size; ++i)
            if (T d = data[i])
                d->DecRef();
    }

    static void Push(T p)
    { if (p) p->IncRef(); }

    static void Pull(T p)
    { if (p) p->DecRef(); }
};

//!
template
    <
        class T,
        template<class> class LifetimePolicy = StandardVectorPolicy,
        int size_of_t = 0
    >
class Vector
{
public:

    enum
    {
        SizeOf_T = size_of_t,
        DefCapacity = 16
    };

    const bool is_t_ptr;

protected:

    T *data;
    int size, capacity;

public:

    Vector()
        : is_t_ptr(IsPointer<T>::val), data(0), size(0), capacity(DefCapacity)
    {
        Allocate(capacity);
    }

    Vector(const int _capacity)
        : is_t_ptr(IsPointer<T>::val), data(0), size(0),
          capacity(_capacity > DefCapacity ? _capacity + DefCapacity : DefCapacity)
    {
        Allocate(capacity);
    }

    Vector(const Vector &v)
        : is_t_ptr(IsPointer<T>::val), data(0), size(0), capacity(0)
    {
        Allocate(v.capacity);
        size = v.size;
        for (int i = 0; i < capacity; ++i)
            data[i] = v[i];
        LifetimePolicy<T>::Copy(data, size);
    }

    ~Vector()
    { Destroy(); }

    Vector &operator =(const Vector &v)
    {
        Destroy();
        Allocate(v.capacity);
        size = v.size;
        for (int i = 0; i < capacity; ++i)
            data[i] = v[i];
        LifetimePolicy<T>::Copy(data, size);
        return *this;
    }

    T &operator [](int index)
    {
        assert(index >= 0 && index < capacity);
        return data[index];
    }

    const T &operator [](int index) const
    {
        assert(index >= 0 && index < capacity);
        return data[index];
    }

    Vector &operator <<(const T &o)
    {
        Push(o);
        return *this;
    }

    bool Allocate(int count)
    {
        Destroy();
        capacity = count < DefCapacity ? DefCapacity : count;
        data = new T[capacity];
        if (data) {
            LifetimePolicy<T>::Init(data, capacity);
            return true;
        }
        return false;
    }

    T *Detach()
    {
        T *t = data;
        data = 0;
        return t;
    }

    bool Reallocate(const int _size)
    {
        size = _size;
        capacity = _size > DefCapacity ? _size + DefCapacity : DefCapacity;
        return Allocate(capacity);
    }

    bool Push(T item)
    {
        if (size == capacity) if (!IncreaseCapacity())
            return false;

        data[size++] = item;
        LifetimePolicy<T>::Push(item);

        return true;
    }

    void Pull()
    {
        LifetimePolicy<T>::Pull(data[size - 1]);
        --size;
    }

    bool IncreaseCapacity(int increase_size = DefCapacity)
    {
        // Create a temporary array with the proper size.
        if (increase_size > DefCapacity)
            increase_size = DefCapacity;
        int new_capacity = capacity + increase_size;
        T *tmp = new T[new_capacity];
        if (!tmp)
            return false;

        LifetimePolicy<T>::Init(tmp, new_capacity);

        for (int i = 0; i < size; i++)
            tmp[i] = data[i];

        // Refresh capacity.
        capacity = new_capacity;

        delete[] data;
        data = tmp;
        tmp = 0;

        return true;
    }

    T *GetData() const
    { return data; }

    T GetData(int i) const
    {
        assert(i >= 0 && i < size);
        return data[i];
    }

    const int Size() const
    { return size; }

    const int GetCapacity() const
    { return capacity; }

    const bool IsNull() const
    { return data == 0; }

    void Destroy()
    {
        LifetimePolicy<T>::Destroy(data, size);
        delete[] data;
        data = 0;
        size = 0;
        capacity = 0;
    }
};

//!
template<class T>
struct AutoVector
{
    typedef Vector<T *, AutoVectorPolicy, sizeof(T)> type;
};
template<class T>
struct SharedVector
{
    typedef Vector<T *, SharedVectorPolicy, sizeof(T)> type;
};

}
}
}
#endif	// __CONTAINERS_VECTOR__
