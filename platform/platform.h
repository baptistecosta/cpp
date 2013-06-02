/**
	engine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_PLATFORM__
#define	__OWL_PLATFORM__

#ifdef		_WIN64 || __x86_64__ || __amd64__
	#define	__PLATFORM_64__
#else
	#define	__PLATFORM_32__
#endif

#endif	//	__OWL_PLATFORM__
