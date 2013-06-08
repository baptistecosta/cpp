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
template<class KType,	class VType, class HashPolicy = StringHasher>
class	HashMap
{
public:
		//!
		class	Pair
		{
				KType			key;
				VType			val;

		public:

				Pair(const KType& _key, const VType& _val) : key(_key), val(_val) {}

				bool			operator ==	(const Pair& entry)		{	return key == entry.GetKey() && val == entry.GetValue();	}

				const KType&	GetKey() const						{	return key;	}
				const VType&	GetValue() const					{	return val;	}

		private:

				void			SetKey(const KType& k)				{	key = k;	}
				void			SetValue(const VType& v)			{	val = v;	}
		};

protected:

		enum
		{	DefaultSize	= 33	};
	
		int				capacity,
						size;
	
		Array<List<Pair> > buckets;

public:

		HashMap(int _size = DefaultSize) : buckets(_size), capacity(_size), size(0) {}

virtual	~HashMap()
		{	Clear();	}

		const VType&	operator []	(const String& k)
		{
			Pair* e = Find(k);
			assert(e);
			return e->GetValue();
		}

virtual	void			Insert(const KType& key, const VType& val)
		{
			int i = GetBucketIndex(key);
			buckets[i].Append(Pair(key, val));
			size++;
		}

virtual	bool			Remove(const KType& key)
		{
			int i = GetBucketIndex(key);
			if (!buckets[i].IsEmpty())
			{
				for (typename List<Pair>::Iterator itr(buckets[i].GetRoot()); itr.IsValid(); ++itr)
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

		Pair*			Find(const KType& key) const
		{
			const List<Pair>& bucket = buckets[GetBucketIndex(key)];
			if (!bucket.IsEmpty())
			{
				for (typename List<Pair>::Iterator itr(bucket.GetRoot()); itr.IsValid(); ++itr)
				{
					if (typename List<Pair>::Node* node = itr.GetNode())
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
			typedef HashMap<KType, VType>::Pair HMPair;
			for (int i = 0; i < capacity; ++i)
			{
				for (List<HMPair>::Iterator itr(buckets[i].getRoot()); itr.isValid(); ++itr)
				{
					if (List<HMPair>::Node* node = itr.getNode())
					{
						if (VType val = node->getData().GetValue())
						{
							processFunc(val);
						}
					}
				}
			}
		}

		const Array<List<Pair> >& GetBuckets() const
		{	return buckets;	}
		const int		GetBucketIndex(const KType& key) const
		{	return HashPolicy::Process(key) % capacity;	}

		const int		GetSize() const
		{	return size;	}
		const int		GetCapacity() const
		{	return capacity;	}
};

//!
template
<
	class KType,
	class VType,
	class HashPolicy = StringHasher,
	template<class> class RefCountPolicy = StandardRefCountPolicy
>
class	SharedHashMap	:	public HashMap<KType, VType, HashPolicy>
{
		typedef				HashMap<KType, VType, HashPolicy>	HM;
		typedef	typename 	HM::Pair						HMEntry;
		typedef	typename 	List<HMEntry>::Node				ListNode;
		typedef	typename 	List<HMEntry>::Iterator			ListIterator;

public:

virtual	~SharedHashMap()
		{	Clear();	}

virtual	void			Insert(const KType& key, const VType& val)
		{
			if (!val)
				return;

			HashMap<KType, VType, HashPolicy>::Insert(key, val);
			StandardRefCountPolicy<VType>::Increment(val);
		}

virtual	bool			Remove(const KType& key)
		{
			if (HMEntry* entry = Find(key))
			{
				VType val = entry->GetValue();
				if (!Remove(key))
					return false;
				StandardRefCountPolicy<VType>::Decrement(val);
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
					for (ListIterator itr = HM::buckets[i].GetRoot(); itr.IsValid(); ++itr)
					{
						if (ListNode* node = itr.GetNode())
						{
							if (VType val = node->GetData().GetValue())
							{
								StandardRefCountPolicy<VType>::Decrement(val);
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
