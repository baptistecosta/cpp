#ifndef __SHARED_OBJECT__
#define __SHARED_OBJECT__

#include <assert.h>
#include "typedefs.h"

namespace BCosta
{
namespace Core
{
namespace Container
{

//!	\brief	Base class for reference counted object used by shared containers.
class SharedObject
{
    int ref_ct;

public:
    SharedObject() : ref_ct(0)
    { }

    virtual    ~SharedObject()
    { }

    int GetRef()
    { return ref_ct; }

    int IncRef()
    { return ++ref_ct; }

    void DecRef()
    {
        if (--ref_ct == 0)
            delete this;
    }

    virtual void Log() const;
};

//!	\brief	Standard reference counting policy for shared containers.
template<class T>
struct StandardRefCountPolicy
{
    static void Increment(T p)
    {
        assert(p);
        p->IncRef();
    }

    static void Decrement(T p)
    {
        assert(p);
        p->DecRef();
    }
};

//!
template
    <
        class T,
        template<class> class RefCountPolicy = StandardRefCountPolicy
    >
struct SharedDataTools
{
    //!	Copy a SharedObject pointer value in an other, automatically incrementing pointed object's reference count if it exists.
    //!	The object pointed by the crushed ReferencedObject pointer will be automatically deleted if his reference count fall to 0.
    static void Copy(T src, T &dest)
    {
        if (src)
            RefCountPolicy<T>::Increment(src);
        if (dest)
            RefCountPolicy<T>::Decrement(dest);
        dest = src;
    }
};

}
}
}
#endif	// __SHARED_OBJECT__