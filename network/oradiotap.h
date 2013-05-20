#ifndef __WLAN_RADIOTAP__
#define __WLAN_RADIOTAP__

	#include <string.h>
	#include "typedefs.h"
	#include "core/containers/vector.h"
	#include "core/bin.h"
	#include "core/log.h"

namespace owl
{

//!
struct	Radiotap
{
		//!
		enum RadiotapFields
		{
			TSFT				= Flag_0,
			FLAGS				= Flag_1,
			RATE				= Flag_2,
			CHANNEL				= Flag_3,
			FHSS				= Flag_4,
			DBM_ANTSIGNAL		= Flag_5,
			DBM_ANTNOISE		= Flag_6,
			LOCK_QUALITY		= Flag_7,
			TX_ATTENUATION		= Flag_8,
			DB_TX_ATTENUATION 	= Flag_9,
			DBM_TX_POWER 		= Flag_10,
			ANTENNA 			= Flag_11,
			DB_ANTSIGNAL 		= Flag_12,
			DB_ANTNOISE 		= Flag_13,
			RX_FLAGS 			= Flag_14,
			TX_FLAGS 			= Flag_15,
			RTS_RETRIES 		= Flag_16,
			DATA_RETRIES 		= Flag_17,
			MCS 				= Flag_19,
			AMPDU_STATUS 		= Flag_20,
			// Valid in every it_present bitmap, even vendor namespaces
			RADIOTAP_NAMESPACE 	= Flag_29,
			VENDOR_NAMESPACE 	= Flag_30,
			EXT 				= Flag_31
		};
		enum
		{
			FLAGS_CFP			= Flag_0,	// sent/received during CFP
			FLAGS_SHORTPRE		= Flag_1,	// sent/received with short preamble
			FLAGS_WEP			= Flag_2,	// sent/received with WEP encryption
			FLAGS_FRAG			= Flag_3,	// sent/received with fragmentation
			FLAGS_FCS			= Flag_4,	// frame includes FCS
			FLAGS_DATAPAD		= Flag_5,	// frame has padding between 802.11 header and payload (to 32-bit boundary)
			FLAGS_BADFCS		= Flag_6,	// frame failed FCS check
			FLAGS_SGI			= Flag_7	// frame used short guard interval (HT)
		};
		enum
		{
			CHANNEL_TURBO		= Flag_4,	// Turbo channel
			CHANNEL_CCK			= Flag_5,	// CCK channel
			CHANNEL_OFDM		= Flag_6,	// OFDM channel
			CHANNEL_2GHZ		= Flag_7,	// 2 GHz spectrum channel
			CHANNEL_5GHZ		= Flag_8,	// 5 GHz spectrum channel
			CHANNEL_PASSIVE		= Flag_9,	// Only passive scan allowed
			CHANNEL_DYN			= Flag_10,	// Dynamic CCK-OFDM channel
			CHANNEL_GFSK		= Flag_11	// GFSK channel (FHSS PHY)
		};
		enum
		{
			FLAG_RX_BADPLCP		= Flag_1	// PLCP CRC check failed
		};
		enum
		{
			MCS_HAVE_BW			= Flag_0,
			MCS_HAVE_MCS		= Flag_1,
			MCS_HAVE_GI			= Flag_2,
			MCS_HAVE_FMT		= Flag_3,
			MCS_HAVE_FEC		= Flag_4
		};
		enum
		{
			MCS_BW_20			= 0,
			MCS_BW_40,
			MCS_BW_20L,
			MCS_BW_20U
		};

		//!
		struct	Entry
		{
				const uint	flag,
							size;

				Entry(uint _flag, uint _size) : flag(_flag), size(_size) {}
		};

		//!
		struct	Header
		{
				uchar		version,
							pad;
				ushort		len;
				uint		present;

				Header()
				{	memset(this, 0, sizeof(*this));	}

				int			Len()
				{	return sizeof(version) + sizeof(pad) + sizeof(len) + sizeof(present);	}
		};

		Vector<Entry*>		ref_entries;
		Header				hdr;

		Radiotap(uchar* frame);

		void				Log(uchar* frame);
};

}		//	owl
#endif	//	__WLAN_RADIOTAP__
