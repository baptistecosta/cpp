/*
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __HASH_TABLE__
#define __HASH_TABLE__

	#include "core/log.h"
	#include "core/string.h"
	#include "array.h"
	#include "defines.h"
	#include "list.h"
	#include "shared_object.h"
	#include "vector.h"

namespace owl {

//!
struct	StringHasher
{
static	ulong			Process(const String&);
};

//!
template<class Key,	class Value, class HashPolicy = StringHasher>
class	HashMap
{
public:
		//!
		class Entry
		{
				Key				key;
				Value			val;

		public:

				Entry(const Key& _key, const Value& _val) : key(_key), val(_val) {}

				bool			operator ==	(const Entry& entry)	{	return key == entry.GetKey() && val == entry.GetValue();	}

				const Key&		GetKey() const						{	return key;	}
				const Value&	GetValue() const					{	return val;	}

		private:

				void			SetKey(const Key& k)				{	key = k;	}
				void			SetValue(const Value& v)			{	val = v;	}
		};

protected:

		typedef typename	HashMap<Key, Value>::Entry	HMEntry;
		typedef typename	List<HMEntry>::Iterator		HMIterator;
		typedef typename	List<HMEntry>::Node			HMNode;

		enum
		{	DefaultSize	= 33	};
	
		int				capacity,
						size;
	
		Array<List<Entry> > buckets;

public:

		HashMap(int _size = DefaultSize) : buckets(_size), capacity(_size), size(0) {}

virtual	~HashMap()
		{	Clear();	}

		const Value&	operator []	(const String& k)
		{
			Entry* e = Find(k);
			assert(e);
			return e->GetValue();
		}

virtual	void			Insert(const Key& key, const Value& val)
		{
			int i = GetBucketIndex(key);
			buckets[i].Append(Entry(key, val));
			size++;
		}

virtual	bool			Remove(const Key& key)
		{
			int i = GetBucketIndex(key);
			if (!buckets[i].IsEmpty())
			{
				for (typename List<Entry>::Iterator itr(buckets[i].GetRoot()); itr.IsValid(); ++itr)
				{
					if (itr.GetNode())
					{
						if (itr.GetNode()->GetData().GetKey() == key)
						{
							buckets[i].Remove(itr.GetNode());
							--size;
							return true;
						}
					}
				}
			}
			return false;
		}

		Entry*			Find(const Key& key) const
		{
			const List<Entry>& bucket = buckets[GetBucketIndex(key)];
			if (!bucket.IsEmpty())
			{
				for (typename List<Entry>::Iterator itr(bucket.GetRoot()); itr.IsValid(); ++itr)
				{
					if (typename List<Entry>::Node* node = itr.GetNode())
					{
						String str = node->GetData().GetKey();
						if (str == key)
							return &itr.GetNode()->GetData();
					}
				}
			}
			return 0;
		}

virtual	void			Clear()
		{
			int n_size = buckets.Size();
			for (int i = 0; i < n_size; ++i)
				buckets[i].Clear();
		}

		template <typename Functor> void ForEachItem(Functor processFunc)
		{
			for (int i = 0; i < capacity; ++i)
			{
				for (HMIterator itr(buckets[i].getRoot()); itr.isValid(); ++itr)
				{
					if (HMNode* node = itr.getNode())
					{
						if (Value val = node->getData().GetValue())
						{
							processFunc(val);
						}
					}
				}
			}
		}

		const Array<List<Entry> >& GetBuckets() const
		{	return buckets;	}
		const int		GetBucketIndex(const Key& key) const
		{	return HashPolicy::Process(key) % capacity;	}

		const int		GetSize() const
		{	return size;	}
		const int		GetCapacity() const
		{	return capacity;	}
};

//!
template
<
	class Key,
	class Value,
	class HashPolicy = StringHasher,
	template<class> class RefCountPolicy = StandardRefCountPolicy
>
class	SharedHashMap	:	public HashMap<Key, Value, HashPolicy>
{
		typedef				HashMap<Key, Value, HashPolicy>	HM;
		typedef	typename 	HM::Entry						HMEntry;
		typedef	typename 	List<HMEntry>::Node				ListNode;
		typedef	typename 	List<HMEntry>::Iterator			ListIterator;

public:

virtual	~SharedHashMap()
		{	Clear();	}

virtual	void			Insert(const Key& key, const Value& val)
		{
			if (!val)
				return;

			HashMap<Key, Value, HashPolicy>::Insert(key, val);
			StandardRefCountPolicy<Value>::Increment(val);
		}

virtual	bool			Remove(const Key& key)
		{
			if (HMEntry* entry = Find(key))
			{
				Value val = entry->GetValue();
				if (!Remove(key))
					return false;
				StandardRefCountPolicy<Value>::Decrement(val);
				return true;
			}
			return false;
		}

virtual	void			Clear()
		{
			int n_size = HM::buckets.Size();
			for (int i = 0; i < n_size; ++i)
			{
				if (!HM::buckets[i].IsEmpty())
				{
					for (ListIterator itr(HM::buckets[i].GetRoot()); itr.IsValid(); ++itr)
					{
						if (ListNode* node = itr.GetNode())
						{
							if (Value val = node->GetData().GetValue())
							{
								StandardRefCountPolicy<Value>::Decrement(val);
							}
						}
					}
				}
			}
			HM::Clear();
		}
};

}		// owl
#endif	// __B_HASH_TABLE__
