/**
	engine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_NETWORK_LLC_HEADER__
#define	__OWL_NETWORK_LLC_HEADER__

	#include <string.h>
	#include "defines.h"

namespace owl {

//!
template<int ControlSize>
struct	LLCHeader
{
		enum
		{
			DSAPAddr_Size	=	1,
			SSAPAddr_Size	=	1,
			Control_Size	=	ControlSize,
			Size			=	DSAPAddr_Size + SSAPAddr_Size + Control_Size
		};

		uchar			dsap_addr,
						ssap_addr,
						control[Control_Size];

public:

		LLCHeader(const uchar* frame)
		{
			memcpy(&dsap_addr, &frame[0], sizeof(uchar));
			memcpy(&ssap_addr, &frame[DSAPAddr_Size], sizeof(uchar));
			memcpy(&control[0], &frame[DSAPAddr_Size + SSAPAddr_Size], sizeof(uchar) * Control_Size);
		}
};

//!
struct	SNAPHeader
{
		enum
		{
			OrgCode_Size	=	3,
			Type_Size		=	2,
			Size			=	OrgCode_Size + Type_Size
		};

		uchar			org_code[OrgCode_Size],
						type[Type_Size];

		SNAPHeader(const uchar* frame)
		{
			memcpy(org_code, &frame[0], sizeof(uchar) * OrgCode_Size);
			memcpy(type, &frame[OrgCode_Size], sizeof(uchar) * Type_Size);
		}
};

}		//	owl
#endif	//	__OWL_NETWORK_LLC_HEADER__
