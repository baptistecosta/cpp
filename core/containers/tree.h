/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __TREE__
#define __TREE__

	#include "double_linked_list.h"

namespace owl {

//!	@class	Tree
template <class	T>	class Tree	:	public	SharedObject
{
private:
	
		typedef Tree<T>		Node;
		typedef DoubleLinkedList_Iterator<Node*> Iterator;

		T					m_data;
		Node*				m_parent;
		DoubleLinkedList<Node*>	m_children;

public:

		Tree()	:	m_parent(NULL)	{}
		~Tree()
		{
			destroy();
		}

		//!	Destroy every child node.
		void				destroy()
		{
			Iterator itr = m_children.iterator();
			Node* node = NULL;
			itr.begin();

			while(itr.isValid())
			{
				node = itr.getNode()->getNodeData();
				m_children.remove(itr);
				delete node;
			}
		}

		//!	Recursively counts the number of nodes in the tree.
		int					size()
		{
			int s = 1;
			Iterator itr = m_children.iterator();
			for(itr.begin(); itr.isValid(); itr++)
				s += itr.Data()->count();
			return s;
		}

		//!	Accessors
		T&					getTreeData()				{	return m_data;	}
		Node*				getParent()				{	return m_parent;	}
		DoubleLinkedList<Node*>& getChildren()		{	return m_children;	}

		void				setParent(Node* parent)	{	m_parent = parent;	}
};


//!	@class	TreeIterator
template<class T>	class TreeIterator
{
		typedef				Tree<T>	Node;

		Node*				m_node;
		DoubleLinkedList_Iterator<Node*> m_child_itr;

public:

		TreeIterator(Node* node = NULL)	:	m_node(node)	{}

		void				operator = (Node* node)
		{
			m_node = node;
			resetIterator();
		}

		//!	Moves the iterator to the root of the tree.
		void				root()
		{
			if (m_node)
				while (m_node->m_parent)
					m_node = m_node->m_parent;

			resetIterator();
		}

		//!	Moves the iterator up or down by one level of the tree.
		void				up()
		{
			if (m_node)
				m_node = m_node->m_parent;

			resetIterator();
		}

		void				down()
		{
			if (m_node)
			{
				m_node = m_child_itr.Data();
				resetIterator();
			}
		}

		//!	Naviguate through children nodes.
		void				nextChild()					{	m_child_itr.next();	}
		void				prevChild()					{	m_child_itr.previous();	}
		void				firstChild()				{	m_child_itr.begin();	}
		void				lastChild()					{	m_child_itr.end();	}

		//!	Determines if the child iterator is valid.
		bool				isChildValid()				{ 	return m_child_itr.isValid();	}

		//!	Return the data the child pointer points to.
		T&					childData() 				{ 	return m_child_itr.getNode()->getNodeData()->getTreeData();	}

		//!	Appends a child to the child list.
		void				appendChild(Node* node) 
		{ 
			if (m_node) 
			{ 
				m_node->getChildren().append(node); 
				node->setParent(m_node);
			} 
		}

		//!	Prepends a child to the child list.
		void				prependChild(Node* node) 
		{ 
			if (m_node) 
			{ 
				m_node->m_children.Prepend(node);
				node->m_parent = m_node;
			} 
		}

		//!	Inserts a new node after the current child 
		void				insertChildAfter(Node* node) 
		{ 
			if (m_node) 
			{ 
				m_node->m_children.InsertAfter(m_child_itr, node);
				node->m_parent = m_node;
			} 
		} 

		//!	Inserts a new node before the current child 
		void				insertChildBefore(Node* node) 
		{ 
			if (m_node) 
			{ 
				m_node->m_children.InsertBefore(m_child_itr, node);
				node->m_parent = m_node;
			} 
		}

		//!	Removes the current child from the tree. Doesn't delete the node.
		void				removeChild()
		{
			if (m_node && m_child_itr.IsValid()) 
			{ 
				m_child_itr.Data()->parent = NULL; 
				m_node->m_children.Remove(m_child_itr); 
			} 
		} 

		//!	Reset the child iterator, so that it points to the right linked list.
		void				resetIterator()
		{
			if (m_node)
				m_child_itr = m_node->m_children.GetIterator();

			else
			{
				m_child_itr.list = NULL;
				m_child_itr.node = NULL;
			}
		}

		//!	Accessors
		Node*				getNode()						{	return m_node;	}
		DoubleLinkedList_Iterator<Node*> getChildItr()		{	return m_child_itr;	}
};

//!	Performs a preorder traversal on a tree.
//!	@param	node		node to start traversing at 
//!	@param	process		function that processes the node 
template<class T> void Preorder(Tree<T>* node, void (*process)(Tree<T>*))
{
	process(node);

	DoubleLinkedList_Iterator<Tree<T>*> itr = node->m_children.GetIterator();

	for(itr.begin(); itr.isValid(); itr++)
		Preorder(itr.Data(), process);
}

//!	Performs a postorder traversal on a tree. 
//!	@param	node		node to start traversing at 
//!	@param	process		function that processes the node 
template<class T> void Postorder(Tree<T>* node, void (*process)(Tree<T>*))
{ 
	DoubleLinkedList_Iterator<Tree<T>*> itr = node->m_children.GetIterator();
	
	for(itr.begin(); itr.isValid(); itr++)
		Postorder(itr.Data(), process);

	process(node); 
}

}		// owl
#endif	// __TREE__
