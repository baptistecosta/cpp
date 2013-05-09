/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __DOUBLE_LINKED_LIST__
#define __DOUBLE_LINKED_LIST__

namespace owl {

template<class T> class DoubleLinkedList_Iterator;

template<class T> class	DLNode
{
		T				m_data;
		DLNode<T>*		m_next;
		DLNode<T>*		m_prev;

public:

		DLNode()
			:	m_data(null_ptr)
			,	m_next(null_ptr)
			,	m_prev(null_ptr)
		{
			//
		}

virtual	~DLNode()	{}

		//!	Detach this node from the list it is in.
		T					detach()
		{
			if (m_prev) m_prev->m_next = m_next;
			if (m_next) m_next->m_prev = m_prev;
			return this->getNodeData();
		}

		//!	Adds a node after the current node.
		void				append(T data)
		{
			DLNode<T>* new_node = new DLNode<T>;
			new_node->m_data = data;

			new_node->m_next = m_next;
			new_node->m_prev = this;

			if (m_next)
				m_next->m_prev = new_node;
			m_next = new_node;
		}

		//!	Adds a node before the current node.
		void				prepend(T data)
		{
			DLNode<T>* new_node = new DLNode<T>;
			new_node->m_data = data;

			new_node->m_next = this;
			new_node->m_prev = m_prev;

			if (m_prev) m_prev->m_next = new_node;
			m_prev = new_node;
		}

		//!	Accessors
		T&					getNodeData()						{	return m_data;	}
		DLNode<T>*			getNext()							{	return m_next;	}
		DLNode<T>*			getPrev()							{	return m_prev;	}

		void				setData(const T& data)				{	m_data = data;	}
		void				setNext(DLNode<T>* next)				{	m_next = next;	}
		void				setPrev(DLNode<T>* prev)				{	m_prev = prev;	}
};

//!	@class	DoubleList
template<class T> class DoubleLinkedList
{
public:

		typedef	DoubleLinkedList_Iterator<T> Iterator;

		DLNode<T>*			m_head;
		DLNode<T>*			m_tail;
		int					m_size;

public:

		DoubleLinkedList()
			:	m_head(null_ptr)
			,	m_tail(null_ptr)
			,	m_size(0)
		{
			//
		}

virtual	~DoubleLinkedList()
		{
			DLNode<T>* node = m_head;
			DLNode<T>* next;

			while (node)
			{
				next = node->getNext();
				delete node;
				node = next;
			}
		}

public:

		//!	Adds a node to the end of the list.
		void				append(T& data)
		{
			if (!m_head)
			{
				m_head = m_tail = new DLNode<T>;
				m_head->setData(data);
				m_head->setNext(null_ptr);
				m_head->setPrev(null_ptr);
			}
			else
			{
				m_tail->append(data);
				m_tail = m_tail->getNext();
			}
			m_size++;
		}

		//!	Adds a node to the beginning of the list.
		void				prepend(T& data)
		{
			if (!m_head)
			{
				m_head = m_tail = createNode();
				m_head->setData(data);
				m_head->setNext(null_ptr);
				m_head->setPrev(null_ptr);
			}
			else
			{
				m_head->prepend(data);
				m_head = m_head->getPrev();
			}
			m_size++;
		}

		//!	Removes the very first node in the list.
		void				removeHead()
		{
			DLNode<T>* node = null_ptr;

			if (m_head)
			{
				node = m_head->getNext();
				delete m_head;
				m_head = node;

				if (!m_head)
					m_tail = null_ptr;
				else
					m_head->setPrev(null_ptr);

				m_size--;
			}
		}

		//!	Removes the very last node in the list.
		void				removeTail()
		{
			DLNode<T>* node = null_ptr;

			if (m_tail)
			{
				node = m_tail->getPrev();

				delete m_tail;
				m_tail = node;

				if (!m_tail)
					m_head = null_ptr;
				else
					m_tail->setNext(null_ptr);

				m_size--;
			}
		}

		//!	Inserts data after the iterator, or at the end.
		void				insertAfter(Iterator& itr, T data)
		{
			if (itr.m_node)
			{
				itr.m_node->InsertAfter(data);

				if (itr.m_node == m_tail)
					m_tail = m_tail->m_next;

				m_size++;
			}
			else
				append(data);
		}

		//!	Inserts data before the iterator, or prepends.
		void				insertBefore(Iterator& itr, T data)
		{
			if (itr.m_node)
			{
				itr.m_node->InsertBefore(data);

				if (itr.m_node == m_head)
					m_head = m_head->m_prev;

				m_size++;
			}
			else
				prepend(data);
		}

		//!	Removes the node that the iterator points to.
		void				remove(Iterator& itr)
		{
			DLNode<T>* node;

			if (!itr.getNode())
				return;

			node = itr.getNode();

			if		(node == m_head) m_head = m_head->getNext();
			else if (node == m_tail) m_tail = m_tail->getPrev();

			itr.next();

			node->detach();
			delete node;

			if (!m_head) m_tail = null_ptr;

			m_size--;
		}

		//!	Accessors
		DLNode<T>*			head()			{	return m_head;	}
		DLNode<T>*			tail()			{	return m_tail;	}
		int					size()			{	return m_size;	}
		Iterator			iterator()		{	return Iterator(this, m_head);	}
};


//!	@class	DoubleListIterator
//-----------------------------------------------------------------------------
template<class T> class DoubleLinkedList_Iterator
{
		typedef DoubleLinkedList_Iterator<T> Iterator;
		
		DoubleLinkedList<T>* m_list;
		DLNode<T>*			m_node;

public:
		Iterator(DoubleLinkedList<T>* list = null_ptr, DLNode<T>* node = null_ptr)
			:	m_list(list)
			,	m_node(m_node)
		{
			//
		}

virtual	~DoubleLinkedList_Iterator()	{}

		Iterator&			operator ++ ()					{	if (m_node) m_node = m_node->getNext(); return *this;	}
		Iterator&			operator --	()					{	if (m_node) m_node = m_node->getPrev(); return *this;	}
		bool				operator == (Iterator& _itr)
		{ 
			if (m_node == _itr.node && m_list == _itr.list)
				return true;

			return false;
		}

		DLNode<T>*			getNode()						{	return m_node;	}
		T&					data()							{	return m_node->getData();	}
		void				begin()							{	if (m_list) m_node = m_list->head();	}
		void				end()							{	if (m_list) m_node = m_list->tail();	}
		void				next()							{	if (m_node) m_node = m_node->getNext();	}
		void				previous()						{	if (m_node) m_node = m_node->getPrev();	}
		bool				isValid()						{	return (m_node != null_ptr);	}
};

}		// owl
#endif	// __DOUBLE_LINKED_LIST__