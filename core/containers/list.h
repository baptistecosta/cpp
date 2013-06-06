/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LIST__
#define __LIST__

	#include <assert.h>
	#include "defines.h"

namespace owl {

//!
template<class T> class List
{
public:
		//!
		class	Node
		{
				friend			class List<T>;

				T				data;
				Node			*next, *prev;
		
		public:
		
				Node(const T& _data) : data(_data), next(0), prev(0) {}
		
				T&				GetData()				{	return data;	}
				Node*			GetNext()				{	return next;	}
				Node*			GetPrev()				{	return prev;	}

		private:

				void			SetNext(Node* n)		{	next = n;	}
				void			SetPrev(Node* p)		{	prev = p;	}
		};

		//!
		class	Iterator
		{
				Node			*cur, *next;

		public:

				Iterator(Node* node = 0)
				{	Reset(node);	}

				Iterator&		operator ++	()
				{
					if (next)
					{
						cur = next;
						next = next->GetNext();
					}
					else
						cur = next = 0;
					
					return *this;
				}

				void			Reset(Node* node = 0)
				{
					cur = node;
					next = cur ? cur->GetNext() : 0;
				};

				bool			IsValid()				{	return cur != 0;	}
				Node*			GetNode()				{	return cur;	}
				Node*			GetNext()				{	return next;	}
		};

protected:

		Node			*root, *last;
		int				size;

public:

		List() : root(0), last(0), size(0) {}
virtual	~List()
		{	Clear();	}

public:

		Node*			NodeAt(int n)
		{
			if (n <= 0 || n > size)
				return 0;
			
			Node* node = root;
			while (n--)
				node = node->GetNext;
			return node;
		}

		T&				operator []	(int n) const	{	return NodeAt(n)->GetData();	}
		T&				DataAt(int n) const			{	return NodeAt(n)->GetData();	}

virtual	Node*			Append(const T& data, Node* ref_node = 0)
		{
			Node* new_node = new Node(data);
			if (!new_node)
				return 0;

			if (!ref_node)
			{
				new_node->SetPrev(last);
				if (last)
					last->SetNext(new_node);
				else
					root = new_node;
				last = new_node;
			}
			else
			{
				if (ref_node->GetNext())
					ref_node->GetNext()->SetPrev(new_node);

				new_node->SetNext(ref_node->GetNext());
				ref_node->SetNext(new_node);
				new_node->SetPrev(ref_node);

				if (last == ref_node)
					last = new_node;
			}

			size++;
			return new_node;
		}

virtual	Node*			Prepend(const T& data, Node* ref_node = 0)
		{
			Node* new_node = new Node(data);
			if (!new_node)
				return 0;

			if (!ref_node)
			{
				new_node->SetNext(root);
				if (root)
					root->SetPrev(new_node);
				else
					last = new_node;
				root = new_node;
			}
			else
			{
				if (ref_node->GetPrev())
					ref_node->GetPrev()->SetNext(new_node);
				new_node->SetPrev(ref_node->GetPrev());
				ref_node->SetPrev(new_node);
				new_node->SetNext(ref_node);

				if (root == ref_node)
					root = new_node;
			}

			size++;
			return new_node;
		}

		T*				Insert(const T& data, int pos)
		{
			Node* ref_node = NodeAt(pos);
			return ref_node ? Prepend(data, ref_node) : 0;
		}

		Node*			Find(const T& data)
		{
			for (Node* cur_node = root; cur_node; cur_node->GetNext())
				if (cur_node->GetData() == data)
					return cur_node;
			return 0;
		}

		bool			BelongsTo(Node* node)
		{
			for (Node* itr_node = root; itr_node; itr_node = itr_node->GetNext())
				if (itr_node == node)
					return true;
			return false;
		}

		Node*			Detach(Node* node)
		{
			if (!node)
				return 0;

			assert(BelongsTo(node));

			if (node->GetPrev())
				node->GetPrev()->SetNext(node->GetNext());
			else
			{
				if (node->GetNext())
					node->GetNext()->SetPrev(0);
				root = node->GetNext();
			}

			if (node->GetNext())
				node->GetNext()->SetPrev(node->GetPrev());
			else
			{
				if (node->GetPrev())
					node->GetPrev()->SetNext(0);
				last = node->GetPrev();
			}

			node->SetPrev(0);
			node->SetNext(0);
		
			--size;
			return node;
		}

virtual	bool			Remove(Node* node)
		{
			if (!Detach(node))
				return false;
			
			delete node;
			return true;
		}

virtual	bool			Remove(const T& data)
		{
			Node* node = Find(data);
			return Remove(node);
		}

virtual	void			Clear()
		{
			Node* next = 0;
			for (Node* node = root; node; node = next)
			{
				next = node->GetNext();
				delete node;
			}

			size = 0;
			root = last = 0;
		}

		const int		Size() const		{	return size;	}
		const bool		IsEmpty() const		{	return size == 0;	}

		Node*			GetRoot() const		{	return root;	}
		Node*			GetLast() const		{	return last;	}
};

//!
template<class T> class SharedList : public List<T>
{
public:

		typedef	typename List<T>::Node	Node;

virtual	~SharedList()
		{	Clear();	}

virtual	Node*			Append(const T& data, Node* ref_node = 0)
		{
			T* new_node = List<T>::Append(data, ref_node);
			if (new_node)
				data->IncRef();
			return new_node;
		}

virtual	Node*			Prepend(const T& data, Node* ref_node = 0)
		{
			T* new_node = List<T>::Prepend(data, ref_node);
			if (new_node)
				data->IncRef();
			return new_node;
		}

virtual	bool			Remove(Node* node)
		{
			if (!node)
				return false;
	
			T data = node->GetData();
			if (!List<T>::Remove(node))
				return false;
			if (data)
				data->DecRef();
			return true;
		}

virtual	bool			Remove(const T& data)
		{
			Node* node = Find(data);
			return SharedList<T>::Remove(node);
		}

virtual	void			Clear()
		{
			for (Node* node = List<T>::root; node; node = node->GetNext())
				node->GetData()->DecRef();
			List<T>::Clear();
		}
};

}		// owl
#endif	// __LIST__
