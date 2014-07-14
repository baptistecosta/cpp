/**
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_CORE_UNCOPYABLE__
#define	__OWL_CORE_UNCOPYABLE__

namespace owl {

//!
class	Uncopyable
{
protected:

		Uncopyable() {}
		~Uncopyable() {}

private:
		
		Uncopyable(const Uncopyable&);
		void			operator =	(const Uncopyable&);
};

}		//	owl
#endif	//	__OWL_CORE_UNCOPYABLE__
