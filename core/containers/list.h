/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LIST__
#define __LIST__

namespace owl {

//!
template<class T> class List
{
public:
	//!
	class	Node
	{
		friend			List<T>;

		T				m_data;
		Node*			m_next, *m_prev;
	
	public:
	
		Node(const T& data)
			:	m_data(data)
			,	m_next(null_ptr)
			,	m_prev(null_ptr)
		{
			//
		}
	
		T&				getData()				{	return m_data;	}
		Node*			getNext()				{	return m_next;	}
		Node*			getPrev()				{	return m_prev;	}

	private:

		void			setNext(Node* next)		{	m_next = next;	}
		void			setPrev(Node* prev)		{	m_prev = prev;	}
	};
	
	//!
	class	Iterator
	{
		Node*			m_cur;
		Node*			m_next;

	public:

		Iterator(Node* node = null_ptr)
		{
			reset(node);
		}

		Iterator&		operator ++	()
		{
			if (m_next)
			{
				m_cur = m_next;

				m_next = m_next->getNext();
			}
			else
				m_cur = m_next = null_ptr;
			
			return *this;
		}

		void			reset(Node* node = null_ptr)
		{
			m_cur = node;
			if (m_cur)
				m_next = m_cur->getNext();
			else
				m_next = null_ptr;

//			m_next = m_cur ? m_cur->getNext() : null_ptr;
		};

		bool			isValid()				{	return m_cur != null_ptr;	}
		Node*			getNode()				{	return m_cur;	}
		Node*			getNext()				{	return m_next;	}
	};

protected:

	int				m_size;
	Node*			m_root;
	Node*			m_last;

public:

	List()
		:	m_root(null_ptr)
		,	m_last(null_ptr)
		,	m_size(0)
	{
		//
	}

	virtual	~List()
	{	clear();	}

public:

	Node*			nodeAt(int n)
	{
		if (n <= 0 || n > m_size)
			return null_ptr;
		
		Node* node = m_root;
		while (n--)
			node = node->getNext;
		return node;
	}

	T&				operator []	(int n) const	{	return nodeAt(n)->getData();	}
	T&				dataAt(int n) const			{	return nodeAt(n)->getData();	}

	virtual	Node*	append(const T& data, Node* ref_node = null_ptr)
	{
		Node* new_node = new Node(data);
		if (!new_node)
			return null_ptr;

		if (!ref_node)
		{
			new_node->setPrev(m_last);
			if (m_last)
				m_last->setNext(new_node);
			else
				m_root = new_node;
			m_last = new_node;
		}
		else
		{
			if (ref_node->getNext())
				ref_node->getNext()->setPrev(new_node);

			new_node->setNext(ref_node->getNext());
			ref_node->setNext(new_node);
			new_node->setPrev(ref_node);

			if (m_last == ref_node)
				m_last = new_node;
		}

		m_size++;
		return new_node;
	}

	virtual	Node*	prepend(const T& data, Node* ref_node = null_ptr)
	{
		Node* new_node = new Node(data);
		if (!new_node)
			return null_ptr;

		if (!ref_node)
		{
			new_node->setNext(m_root);
			if (m_root)
				m_root->setPrev(new_node);
			else
				m_last = new_node;
			m_root = new_node;
		}
		else
		{
			if (ref_node->getPrev())
				ref_node->getPrev()->setNext(new_node);
			new_node->setPrev(ref_node->getPrev());
			ref_node->setPrev(new_node);
			new_node->setNext(ref_node);

			if (m_root == ref_node)
				m_root = new_node;
		}

		m_size++;
		return new_node;
	}

	T*				insert(const T& data, int pos)
	{
		Node* ref_node = nodeAt(pos);
		return ref_node ? prepend(data, ref_node) : null_ptr;
	}

	Node*			find(const T& data)
	{
		for (Node* cur_node = m_root; cur_node; cur_node->getNext())
			if (cur_node->getData() == data)
				return cur_node;
		return null_ptr;
	}

	bool			belongsTo(Node* node)
	{
		for (Node* itr_node = m_root; itr_node; itr_node = itr_node->getNext())
			if (itr_node == node)
				return true;
		return false;
	}

	Node*			detach(Node* node)
	{
		if (!node)
			return null_ptr;

		assert(belongsTo(node));

		if (node->getPrev())
			node->getPrev()->setNext(node->getNext());
		else
		{
			if (node->getNext())
				node->getNext()->setPrev(null_ptr);
			m_root = node->getNext();
		}

		if (node->getNext())
			node->getNext()->setPrev(node->getPrev());
		else
		{
			if (node->getPrev())
				node->getPrev()->setNext(null_ptr);
			m_last = node->getPrev();
		}

		node->setPrev(null_ptr);
		node->setNext(null_ptr);
	
		--m_size;
		return node;
	}

	virtual	bool	remove(Node* node)
	{
		if (!detach(node))
			return false;
		
		delete node;
		return true;
	}

	virtual	bool	remove(const T& data)
	{
		Node* node = find(data);
		return remove(node);
	}

	virtual	void	clear()
	{
		Node* next = null_ptr;
		for (Node* node = m_root; node; node = next)
		{
			next = node->getNext();
			delete node;
		}

		m_size = 0;
		m_root = m_last = null_ptr;
	}

	int				size() const
	{	return m_size;	}
	bool			isEmpty() const
	{	return m_size == 0;	}

	Node*			getRoot()
	{	return m_root;	}
	Node*			getLast()
	{	return m_last;	}
};

//!
template<class T> class SharedList :	public	List<T>
{
public:

	virtual	~SharedList()
	{	clear();	}

	virtual	Node*			append(const T& data, Node* ref_node = null_ptr)
	{
		T* new_node = List<T>::append(data, ref_node);
		if (new_node)
			data->incRef();
		return new_node;
	}

	virtual	Node*			prepend(const T& data, Node* ref_node = null_ptr)
	{
		T* new_node = List<T>::prepend(data, ref_node);
		if (new_node)
			data->incRef();
		return new_node;
	}

	virtual	bool			remove(Node* node)
	{
		if (!node)
			return false;

		T data = node->getData()
		if (!List<T>::remove(node))
			return false;
		if (data)
			data->decRef();
		return true;
	}

	virtual	bool			remove(const T& data)
	{
		Node* node = find(data);
		return SharedList<T>::remove(node);
	}

	virtual	void			clear()
	{
		for (Node* node = m_root; node; node = node->getNext())
			node->getData()->decRef();
		List<T>::clear();
	}
};

}		// owl
#endif	// __LIST__