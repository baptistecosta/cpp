#ifndef BCOSTA_CORE_CONTAINER_SHARED_PTR_H
#define BCOSTA_CORE_CONTAINER_SHARED_PTR_H

#include "shared_object.h"

namespace BCosta {
namespace Core {
namespace Container {

template
    <
        class T, template<class> class RefCountingPolicy = StandardRefCountPolicy
    >
class SharedPtr
{
    T *ptr;

public:

    SharedPtr() : ptr(0)
    { }

    SharedPtr(T *ptr) : ptr(0)
    { Init(ptr); }

    SharedPtr(const SharedPtr &shptr) : ptr(0)
    { Init(const_cast<T *>(shptr.Raw())); }

    ~SharedPtr()
    {
        if (ptr) {
            RefCountingPolicy<T *>::Decrement(ptr);
        }
    }

private:

    void Init(T *_ptr)
    {
        if (ptr = _ptr) {
            RefCountingPolicy<T *>::Increment(ptr);
        }
    }

public:
    // Operators
    T *operator ->()
    { return ptr; }

    const T *operator ->() const
    { return ptr; }

    T &operator *()
    { return *ptr; }

    SharedPtr &operator =(const SharedPtr &shptr)
    { return *this = const_cast<T *>(shptr.Raw()); }

    SharedPtr &operator =(T *_ptr)
    {
        SharedDataTools<T *>::Copy(_ptr, ptr);
        return *this;
    }

    // Get raw pointer.
    T *Raw()
    { return ptr; }

    const T *Raw() const
    { return ptr; }

    // Is pointer non null.
    const bool IsValid() const
    { return ptr != 0; }

    // Is pointer null.
    const bool IsNull() const
    { return ptr == 0; }
};

}
}
}
#endif // BCOSTA_CORE_CONTAINERS_SHARED_PTR_H
