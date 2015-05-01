#ifndef BCOSTA_CORE_CONTAINER_AUTO_PTR_H
#define BCOSTA_CORE_CONTAINER_AUTO_PTR_H

namespace BCosta {
namespace Core {
namespace Container {

template<class T>
class AutoPtr
{
    T *p;

public:
    AutoPtr(T *_p = 0) : p(_p)
    { }

    ~AutoPtr()
    {
        delete p;
        p = 0;
    }

    operator T *() const
    { return p; }

    T *operator ->() const
    { return p; }

    T &operator *() const
    { return *p; }

    T &operator [](unsigned int n) const
    { return p[n]; }

    T *operator =(T *_p)
    {
        if (p != _p) {
            delete p;
            p = _p;
            _p = 0;
        }
        return p;
    }

    bool operator ==(const T *_p) const
    { return p == _p; }

    bool operator !=(const T *_p) const
    { return p != _p; }

    T *Raw()
    { return p; }

    const T *Raw() const
    { return p; }

    T *Detach()
    {
        T *d = p;
        p = 0;
        return d;
    }

    const bool IsNull() const
    { return p == 0; }

    const bool IsValid() const
    { return p != 0; }
};

}
}
}
#endif // BCOSTA_CORE_CONTAINER_AUTO_PTR_H