/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OWL_CORE_CONTAINERS_TREE__
#define __OWL_CORE_CONTAINERS_TREE__

	#include "list.h"

namespace owl {

//!
template <class	T>	class Tree
{
		typedef	typename List<Tree<T>*>::Iterator TreeListIterator;

public:

		class	Iterator
		{
				Tree<T>*			tree;
				TreeListIterator	sons_itr;

		public:

				Iterator(Tree<T>* _tree = 0) : tree(_tree) {}

				void				operator = (Tree<T>* _tree)
				{
					tree = _tree;
					ResetIterator();
				}

				//!	Moves the iterator to the root of the tree.
				void				Root()
				{
					if (tree)
						while (tree->parent)
							tree = tree->parent;
					ResetIterator();
				}

				//!	Moves the iterator up or down by one level of the tree.
				void				Up()
				{
					if (tree)
						tree = tree->parent;

					ResetIterator();
				}
				void				Down()
				{
					if (tree)
					{
						tree = sons_itr.GetNode();
						ResetIterator();
					}
				}

				void				NextChild()					{	sons_itr.GetNext();	}
				bool				IsChildValid()				{ 	return sons_itr.IsValid();	}

				T&					SonData() 					{ 	return sons_itr.GetNode()->GetData();	}

				void				AppendChild(Tree<T>* _tree)
				{
					if (tree)
					{
						tree->GetSons().Append(_tree);
						_tree->SetParent(tree);
					}
				}

				void				PrependChild(Tree<T>* _tree)
				{
					if (tree)
					{
						tree->sons.Prepend(_tree);
						_tree->parent = tree;
					}
				}

				void				RemoveChild()
				{
					if (tree && sons_itr.IsValid())
						sons_itr.GetNode()->GetData()->Clear();
				}

				//!	Reset the child iterator, so that it points to the right linked list.
				void				ResetIterator()
				{
					if (tree)
						sons_itr = tree->GetSons().GetIterator();
					else
						sons_itr.Reset();
				}

				Tree<T>*				GetNode()				{	return tree;	}
				TreeListIterator		GetSonsIterator()		{	return sons_itr;	}
		};

private:

		T					data;
		Tree<T>*			parent;
		List<Tree<T>*>		sons;

		Iterator			it;

public:

		Tree() : parent(0) {}
		~Tree()
		{	Clear();	}

		void				Clear()
		{
			it.RemoveChild();
			for (TreeListIterator it = sons.GetRoot(); it.IsValid(); ++it)
			{
				auto t = it.GetNode()->GetData();
				
			}
		}

		int					Size()
		{
			int s = 1;
			Iterator itr = sons.iterator();
			for(itr.begin(); itr.isValid(); itr++)
				s += itr.Data()->count();
			return s;
		}

		void				AppendChild(Tree<T>* node) 
		{ 
			if (m_node)
			{ 
				m_node->GetSons().append(node); 
				node->SetParent(m_node);
			} 
		}

		T&					GetData()					{	return data;	}
		Tree<T>*			GetParent()					{	return parent;	}
		List<Tree<T>*>&		GetSons()					{	return sons;	}
		Iterator			GetIterator()				{	return it;	}

		void				SetParent(Tree<T>* parent)	{	parent = parent;	}
};


//!

}		//	owl
#endif	//	__OWL_CORE_CONTAINERS_TREE__
