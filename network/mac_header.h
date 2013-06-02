#ifndef __WLAN_MAC_HEADER__
#define __WLAN_MAC_HEADER__

	#include "core/containers/vector.h"
	#include "core/bin.h"
	#include "defines.h"

namespace owl
{

//!
struct	MACHeader
{
		//!
		struct	Field
		{
				int				id, bytes;
				Field(int _id, int _bytes) : id(_id), bytes(_bytes) {}
		};

		//!
		enum	FieldsID
		{
			FieldID_FrameCtrl	=	0,
			FieldID_DurationID,
			FieldID_Addr1,
			FieldID_Addr2,
			FieldID_Addr3,
			FieldID_SeqCtrl,
			FieldID_Addr4,
			FieldID_QoSControl,
			FieldID_HTCtrl,
			FieldID_FrameBody,
			FieldID_FCS
		};
		//!
		enum	FieldsSize
		{
			FieldSize_FrameCtrl		=	2,
			FieldSize_DurationID	=	2,
			FieldSize_Addr			=	6,
			FieldSize_SeqCtrl		=	2,
			FieldSize_QoSCtrl		=	2,
			FieldSize_HTCtrl		=	4,
			FieldSize_FrameBody		=	-1,
			FieldSize_FCS			=	4
		};

		//!
		struct	FrameControl
		{
				//!
				enum	ManagementTypeValue
				{
					AssocRequest		=	0x0,
					AssocRes			=	0x1,
					ReassocReq			=	0x2,
					ReassocRes			=	0x3,
					ProbeReq			=	0x4,
					ProbeRes			=	0x5,
					Beacon				=	0x8,
					ATIM				=	0x9,
					Disassoc			=	0xa,
					Auth				=	0xb,
					Deauth				=	0xc,
					Action				=	0xd
				};
				enum	ControlTypeValue
				{
					BlockAckReq			=	0x8,
					BlockAck			=	0x9,
					PS_Poll				=	0xa,
					RTS					=	0xb,
					CTS					=	0xc,
					ACK					=	0xd,
					CFEnd				=	0xe,
					CFEnd_CFAck			=	0xf
				};
				enum	DataTypeValue
				{
					Data				=	0x0,	// 0000
					Data_CFAck			=	0x1,	// 0001
					Data_CFPoll			=	0x2,	// 0010
					Data_CFAck_CFPoll	=	0x3,	// 0011
					Null				=	0x4,	// 0100
					CFAck				=	0x5,	// 0101
					CFPoll				=	0x6,	// 0110
					CFAck_CFPoll		=	0x7,	// 0111
					QoSMask				=	0x8,	// 1000
				};

				//!
				enum	Flags
				{
					ToDS				=	Flag_0,
					FromDS				=	Flag_1,
					MoreFrag			=	Flag_2,
					Retry				=	Flag_3,
					PowerMgmt			=	Flag_4,
					MoreData			=	Flag_5,
					WEP					=	Flag_6,
					Order				=	Flag_7
				};

				unsigned		version	:	2,
								type	:	2,
								subtype	:	4;
				uchar			flags;

				FrameControl() : version(0), type(0), subtype(0), flags(0) {}
		};

		//!
		struct	SequenceControl
		{
				unsigned		frag_num	:	4,
								seq_num		:	12;

				SequenceControl() : frag_num(0), seq_num(0) {}
		};

		//!
		enum	FrameType
		{	Management = 0, Control, Data	};

		Vector<MACHeader::Field*> fields;
		int				size;

		uchar*			frame;

		FrameControl	frame_ctrl;
		ui16			duration_id;
		ui8				addr_1[6],
						addr_2[6],
						addr_3[6];
		SequenceControl	seq_ctrl;
		uchar			addr_4[6];

		ui32			fcs;

		MACHeader(uchar* frame);

		void			BuildFrameFormat();
		void			Parse();
};

}
#endif	//	__WLAN_MAC_HEADER__
