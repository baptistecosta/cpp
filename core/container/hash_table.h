#ifndef BCOSTA_CORE_CONTAINER_HASH_TABLE_H
#define BCOSTA_CORE_CONTAINER_HASH_TABLE_H

#include "core/string.h"
#include "array.h"
#include "defines.h"
#include "list.h"
#include "shared_object.h"
#include "vector.h"

namespace BCosta {
namespace Core {
namespace Container {

struct StringHasher
{
    static unsigned long Process(const String &);
};

template<class T>
struct StandardHMPol
{
    static void OnInit(T)
    { }

    static void OnInsert(T)
    { }

    static bool OnRemove(T)
    { return true; }
};

template<class T>
struct AutoHMPol
{
    static void OnInit(T d)
    { d = 0; }

    static void OnInsert(T)
    { }

    static bool OnRemove(T d)
    {
        delete d;
        return true;
    }
};

template<class T>
struct SharedHMPol
{
    static void OnInit(T d)
    { d = 0; }

    static void OnInsert(T d)
    { if (d) StandardRefCountPolicy<T>::Increment(d); }

    static bool OnRemove(T d)
    {
        StandardRefCountPolicy<T>::Decrement(d);
        return true;
    }
};

template
    <
        class KType,
        class VType,
        template<class> class MemoryPolicy = StandardHMPol,
        class HashPolicy = StringHasher
    >
class HashMap
{
public:
    class Pair
    {
        KType key;
        VType val;

    public:
        Pair(const KType &_key, const VType &_val) : key(_key), val(_val)
        { }

        bool operator ==(const Pair &entry)
        { return key == entry.GetKey() && val == entry.GetValue(); }

        const KType &GetKey() const
        { return key; }

        const VType &GetValue() const
        { return val; }

    private:
        void SetKey(const KType &k)
        { key = k; }

        void SetValue(const VType &v)
        { val = v; }
    };

protected:

    enum
    {
        DefaultSize = 33
    };

    int capacity,
        size;

    Array <List<Pair>> buckets;

public:

    HashMap(int _size = DefaultSize) : buckets(_size), capacity(_size), size(0)
    {
        ForEach([](KType k, VType v) { MemoryPolicy<VType>::OnInit(v); });
    }

    ~HashMap()
    { Destroy(); }

    HashMap &operator =(const HashMap &m)
    {
        Destroy();
        capacity = capacity;
        m.ForEach([this](const KType &k, VType &v) { Insert(k, v); });
        return *this;
    }

    const VType &operator [](const String &k) const
    {
        Pair *e = Find(k);
        assert(e);
        return e->GetValue();
    }

    void Insert(const KType &key, const VType &val)
    {
        int i = GetBucketIndex(key);
        buckets[i].Append(Pair(key, val));
        size++;
        MemoryPolicy<VType>::OnInsert(val);
    }

    bool Remove(const KType &key)
    {
        bool r = false;
        ForEachNodeInBucket(key, [&](const List <Pair> &bucket, class List<Pair>::Node &n) -> bool {
            if (n.GetData().GetKey() == key) if (MemoryPolicy<VType>::OnRemove(n.GetData().GetValue())) {
                r = const_cast<List <Pair> & >(bucket).Remove(&n);
            }
            return r;
        });
        return r;
    }

    Pair *Find(const KType &key) const
    {
        Pair *p = 0;
        ForEachNodeInBucket(key, [&](const List <Pair> &bucket, class List<Pair>::Node &n) -> bool {
            if (n.GetData().GetKey() == key) {
                p = &n.GetData();
                return true;
            }
            return false;
        });
        return p;
    }

    void Destroy()
    {
        ForEach([](const KType &k, VType &v) { MemoryPolicy<VType>::OnRemove(v); });
        for (int i = 0; i < buckets.Size(); ++i) {
            buckets[i].Clear();
        }
    }

    template<typename LambdaExp>
    void ForEach(LambdaExp func)
    { static_cast<const HashMap &>(*this).ForEach(func); }

    template<typename LambdaExp>
    void ForEach(LambdaExp func) const
    {
        ForEachNode([&func](class List<Pair>::Node *n) -> bool {
            KType k = n->GetData().GetKey();
            if (VType v = n->GetData().GetValue())
                func(k, v);
            return false;
        });
    }

    template<typename LambdaExp>
    void ForEachNode(LambdaExp func)
    { static_cast<const HashMap &>(*this).ForEachNode(func); }

    template<typename LambdaExp>
    void ForEachNode(LambdaExp func) const
    {
        for (int i = 0; i < capacity; ++i) {
            if (class List<Pair>::Node *r = buckets[i].GetRoot()) {
                for (class List<Pair>::Iterator itr = r; itr.IsValid(); ++itr) {
                    if (class List<Pair>::Node *n = itr.GetNode()) if (func(n)) {
                        return;
                    }
                }
            }
        }
    }

    template<typename LambdaExp>
    void ForEachNodeInBucket(String key, LambdaExp func)
    { static_cast<const HashMap &>(*this).ForEachNodeInBucket(func); }

    template<typename LambdaExp>
    void ForEachNodeInBucket(String key, LambdaExp func) const
    {
        const List <Pair> &bucket = buckets[GetBucketIndex(key)];
        if (!bucket.IsEmpty()) if (class List<Pair>::Node *r = bucket.GetRoot())
            for (class List<Pair>::Iterator itr = r; itr.IsValid(); ++itr)
                if (class List<Pair>::Node *n = itr.GetNode()) if (func(bucket, *n))
                    return;
    }

    const Array <List<Pair>> &GetBuckets() const
    { return buckets; }

    const int GetBucketIndex(const KType &key) const
    { return HashPolicy::Process(key) % capacity; }

    const int GetSize() const
    { return size; }

    const int GetCapacity() const
    { return capacity; }
};

template<class KType, class VType>
struct AutoHashMap
{
    typedef HashMap<KType, VType *, AutoHMPol> type;
};
template<class KType, class VType>
struct SharedHashMap
{
    typedef HashMap<KType, VType *, SharedHMPol> type;
};

}
}
}
#endif // BCOSTA_CORE_CONTAINER_HASH_TABLE_H