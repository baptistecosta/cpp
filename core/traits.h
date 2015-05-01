#ifndef __TRAITS__
#define __TRAITS__

namespace BCosta
{
namespace Core
{

// IsPointer traits
template<typename T>
struct IsPointer
{
    static const bool val = false;
};
template<typename T>
struct IsPointer<T *>
{
    static const bool val = true;
};

// TEST
template<int N>
struct TemplateInt
{
    static const int val = N;
};

}
}
#endif	// __TRAITS__