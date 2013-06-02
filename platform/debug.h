/*
	bEngine :: 2011 - 2013s
	Baptiste Costa
*/

#ifndef	__OWL_DEBUG__
#define	__OWL_DEBUG__

#ifdef _DEBUG		// Windows
	#define __debug__	1
#else
	#ifndef NDEBUG	// Linux
		#define __debug__	1
	#else
		#define __debug__	0
	#endif
#endif

#endif	//	__OWL_DEBUG__
