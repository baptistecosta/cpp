/*
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __HASH_TABLE__
#define __HASH_TABLE__

	#include "array.h"
	#include "list.h"
	#include "shared_object.h"
	#include "vector.h"
	#include "core\log.h"
	#include "core\string.h"

namespace owl {

//!
struct	StringHasher
{
static	unsigned long int process(const String& str);
};

//!
template
<
	class Key
,	class Value
,	class HashPolicy = StringHasher
>
class	HashMap
{
public:
	//!
	class Entry
	{
		Key				m_key;
		Value			m_value;

	public:

		Entry(const Key& key, const Value& val)
			:	m_key(key)
			,	m_value(val)
		{
			//
		}

		bool			operator ==	(const Entry& entry)
		{	return m_key == entry.getKey() && m_value == entry.getValue();	}

		const Key&		getKey() const					{	return m_key;	}
		const Value&	getValue() const				{	return m_value;	}

	private:

		void			setKey(const Key& key)			{	m_key = key;	}
		void			setValue(const Value& val)		{	m_value = val;	}
	};

protected:

	enum
	{	DefaultSize	= 33	};

	int				m_capacity
				,	m_size;

	Array<List<Entry>> m_buckets;

public:

	HashMap(int size = DefaultSize)
		:	m_buckets(size)
		,	m_capacity(size)
		,	m_size(0)
	{
		//
	}

	virtual	~HashMap()
	{	clear();	}

	const Value&	operator []	(const String& k)
	{
		Entry* e = find(k);
		assert(e);
		return e->getValue();
	}

	virtual	void	insert(const Key& key, const Value& val)
	{
		int i = getBucketIndex(key);
		m_buckets[i].append(Entry(key, val));
		m_size++;
	}

	virtual	bool	remove(const Key& key)
	{
		int i = getBucketIndex(key);
		if (!m_buckets[i].isEmpty())
		{
			for (List<Entry>::Iterator itr(m_buckets[i].getRoot()); itr.isValid(); ++itr)
			{
				if (itr.getNode())
				{
					if (itr.getNode()->getData().getKey() == key)
					{
						m_buckets[i].remove(itr.getNode());
						--m_size;
						return true;
					}
				}
			}
		}
		return false;
	}

	Entry*			find(const Key& key)
	{
		List<Entry>& bucket = m_buckets[getBucketIndex(key)];
		if (!bucket.isEmpty())
		{
			for (List<Entry>::Iterator itr(bucket.getRoot()); itr.isValid(); ++itr)
			{
				if (List<Entry>::Node* node = itr.getNode())
				{
					String str = node->getData().getKey();
					if (str == key)
						return &itr.getNode()->getData();
				}
			}
		}
		return NULL;
	}

	virtual	void	clear()
	{
		int n_size = m_buckets.size();
		for (int i = 0; i < n_size; ++i)
			m_buckets[i].clear();
	}

	template <typename Functor> void forEachItem(Functor processFunc)
	{
		for (int i = 0; i < m_capacity; ++i)
		{
			for (List<HashMap<Key, Value>::Entry>::Iterator itr(m_buckets[i].getRoot()); itr.isValid(); ++itr)
			{
				if (auto node = itr.getNode())
				{
					if (Value val = node->getData().getValue())
					{
						processFunc(val);
					}
				}
			}
		}
	}

	const Array<List<Entry>>& getBuckets() const
	{	return m_buckets;	}
	const int		getBucketIndex(const Key& key) const
	{	return HashPolicy::process(key) % m_capacity;	}

	const int		getSize() const
	{	return m_size;	}
	const int		getCapacity() const
	{	return m_capacity;	}
};

//!
template
<
	class Key
,	class Value
,	class HashPolicy						=	StringHasher
,	template<class> class RefCountPolicy	=	StandardRefCountPolicy
>
class	SharedHashMap	:	public HashMap<Key, Value, HashPolicy>
{
public:

	virtual	~SharedHashMap()
	{	clear();	}

	virtual	void	insert(const Key& key, const Value& val)
	{
		if (!val)
			return;

		HashMap<Key, Value, HashPolicy>::insert(key, val);
		StandardRefCountPolicy<Value>::Increment(val);
	}

	virtual	bool	remove(const Key& key)
	{
		if (Entry* entry = find(key))
		{
			Value val = entry->getValue();
			if (!remove(key))
				return false;
			StandardRefCountPolicy<Value>::Decrement(val);
			return true;
		}
		return false;
	}

	virtual	void	clear()
	{
		int n_size = m_buckets.size();
		for (int i = 0; i < n_size; ++i)
		{
			if (!m_buckets[i].isEmpty())
			{
				for (List<Entry>::Iterator itr(m_buckets[i].getRoot()); itr.isValid(); ++itr)
				{
					if (auto node = itr.getNode())
					{
						if (Value val = node->getData().getValue())
							StandardRefCountPolicy<Value>::Decrement(val);
					}
				}
			}
		}
		HashMap<Key, Value, HashPolicy>::clear();
	}
};

}		// owl
#endif	// __B_HASH_TABLE__