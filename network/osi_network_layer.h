/**
	engine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_NETWORK_OSI_NETWORK_LAYER__
#define	__OWL_NETWORK_OSI_NETWORK_LAYER__

	#include "defines.h"

namespace owl {

//!
struct	EAPOL
{
		enum
		{
			Version_Size			=	1,
			PacketType_Size			=	1,
			PacketBodyLenght_Size	=	2,
			DescriptorType_Size		=	1,
			KeyInfo_Size			=	2,
			KeyLen_Size				=	2,
			KeyReplayCounter_Size	=	8,
			KeyNonce_Size			=	32,
			KeyIV_Size				=	16,
			KeyRSC_Size				=	8,
			KeyID_Size				=	8,
			KeyMIC_Size				=	16,
			KeyDataLength_Size		=	2,
			Size					=	123
		};

		ui8				frame[Size];

		ui8				version,
						pck_type;
		ui16			pck_body_len;
		ui8				descriptor_type;
		ui16			key_info,
						key_len;
		ui64			replay_counter;
		ui8				key_nounce[KeyNonce_Size],
						key_iv[KeyIV_Size];
		ui64			key_rsc,
						key_id;
		ui8				key_mic[KeyMIC_Size];
		ui16			key_data_len;
		ui8*			key_data;
		
public:

		EAPOL(const ui8* frame);
};

}		//	owl
#endif	//	__OWL_NETWORK_OSI_NETWORK_LAYER__
