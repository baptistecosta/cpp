#ifndef BCOSTA_CORE_UNCOPYABLE_H
#define BCOSTA_CORE_UNCOPYABLE_H

namespace BCosta {
namespace Core {

class Uncopyable
{
protected:
    Uncopyable()
    { }

    ~Uncopyable()
    { }

private:
    Uncopyable(const Uncopyable &);

    void operator =(const Uncopyable &);
};

}
}
#endif // BCOSTA_CORE_UNCOPYABLE_H
