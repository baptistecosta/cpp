	#include <stddef.h>
	#include <string.h>
	#include "mac_header.h"
	#include "core/log.h"

	using namespace owl;

//-----------------------------------------------------------------------------
MACHeader::MACHeader(uchar* _frame)
	: frame(_frame)
	, size(0)
	, duration_id(0)
	, fcs(0)
{
	for (int i = 0; i < 6; ++i)
		addr_1[i] = addr_2[i] = addr_3[i] = addr_4[i] = 0;

	memcpy(&frame_ctrl, &frame[0], 1);
	memcpy(&frame_ctrl.flags, &frame[1], 1);
}

//-----------------------------------------------------------------------------
void			MACHeader::BuildFrameFormat()
{
	typedef	FrameControl		FC;
	typedef	MACHeader::Field	MF;

	fields
		<< new MF(FieldID_FrameCtrl, FieldSize_FrameCtrl)
		<< new MF(FieldID_DurationID, FieldSize_DurationID)
		<< new MF(FieldID_Addr1, FieldSize_Addr);

	switch (frame_ctrl.type)
	{
		case Management:
			fields
				<< new MF(FieldID_Addr2, FieldSize_Addr)
				<< new MF(FieldID_Addr3, FieldSize_Addr)
				<< new MF(FieldID_SeqCtrl, FieldSize_SeqCtrl);

			if (!(frame_ctrl.subtype & FC::QoSMask) && (frame_ctrl.flags & FC::Order))
			{
				// Frame contain an MSDU transfered using the StrictlyOrdered service class
			}
			else if ((frame_ctrl.subtype & FC::QoSMask) && (frame_ctrl.flags & FC::Order))
				fields << new MF(FieldID_HTCtrl, FieldSize_HTCtrl);

			break;

		case Control:
			switch (frame_ctrl.subtype)
			{
				case FC::BlockAckReq:
				case FC::BlockAck:
				case FC::PS_Poll:
				case FC::RTS:
				case FC::CFEnd:
				case FC::CFEnd_CFAck:
					fields << new MF(FieldID_Addr2, FieldSize_Addr);
					break;
			}
			break;

		case Data:
			fields
				<< new MF(FieldID_Addr2, FieldSize_Addr)
				<< new MF(FieldID_Addr3, FieldSize_Addr)
				<< new MF(FieldID_SeqCtrl, FieldSize_SeqCtrl);

			if ((frame_ctrl.flags & FC::ToDS) && (frame_ctrl.flags & FC::FromDS))
				fields << new MF(FieldID_Addr4, FieldSize_Addr);

			break;
	}
	fields << new MF(FieldID_FCS, FieldSize_FCS);

	for (int i = 0; i < fields.Size(); ++i)
		size += fields[i]->bytes;
}

//-----------------------------------------------------------------------------
void			MACHeader::Parse()
{
	typedef	FrameControl FC;

	if (frame_ctrl.type == Management && frame_ctrl.subtype == FC::Beacon)
		return;

	Log::NewLine();

	int offset = 0;
	for(int i = 0; i < fields.Size(); ++i)
	{
		MACHeader::Field* f = fields[i];
		switch (f->id)
		{
			case FieldID_FrameCtrl:
				__LOG("Frame control data:")
				__LOG("Version = %d", frame_ctrl.version);

				switch (frame_ctrl.type)
				{
					case Management:
						__LOG("Type: Management")
						switch (frame_ctrl.subtype)
						{
							case FC::AssocRes: __LOG("Subtype: Association response") break;
							case FC::ReassocReq: __LOG("Subtype: Reassociation request") break;
							case FC::ReassocRes: __LOG("Subtype: Reassociation response") break;
							case FC::ProbeReq: __LOG("Subtype: Probe request") break;
							case FC::ProbeRes: __LOG("Subtype: Probe response") break;
							case FC::Beacon: __LOG("Subtype: Beacon") break;
							case FC::ATIM: __LOG("Subtype: ATIM") break;
							case FC::Disassoc: __LOG("Subtype: Disassociation") break;
							case FC::Auth: __LOG("Subtype: Authentication") break;
							case FC::Deauth: __LOG("Subtype: Deauthentication") break;
							case FC::Action: __LOG("Subtype: Action") break;
						}
						break;

					case Control:
						__LOG("Type: Control");
						switch (frame_ctrl.subtype)
						{
							case FC::BlockAckReq: __LOG("Subtype: Block acknowledgement request") break;
							case FC::BlockAck: __LOG("Subtype: Block acknowledgement") break;
							case FC::PS_Poll: __LOG("Subtype: PS-Poll") break;
							case FC::RTS: __LOG("Subtype: Request-to-send frame (RTS)") break;
							case FC::CTS: __LOG("Subtype: Clear-to-send frame (CTS)") break;
							case FC::ACK: __LOG("Subtype: Acknowledge frame (ACK)") break;
							case FC::CFEnd: __LOG("CF-end") break;
							case FC::CFEnd_CFAck: __LOG("CF-end + CF-ack") break;
						}
						break;

					case Data:
						__LOG("Type: Data")
						switch (frame_ctrl.subtype)
						{
							case FC::Data: __LOG("Data frame") break;
							case FC::Data_CFAck: __LOG("Data frame + CF-ack") break;
							case FC::Data_CFPoll: __LOG("Data frame + CF-poll") break;
							case FC::Data_CFAck_CFPoll: __LOG("Data frame + CF-ack + CF-poll") break;
							case FC::Null: __LOG("Null frame") break;
							case FC::CFAck: __LOG("CF-ack") break;
							case FC::CFPoll: __LOG("CF-poll") break;
							case FC::CFAck_CFPoll: __LOG("CF-ack + CF-poll") break;
							case (FC::QoSMask | FC::Data): __LOG("QoS version of the data frame") break;
							case (FC::QoSMask | FC::Data_CFAck): __LOG("QoS data + CF-ack") break;
							case (FC::QoSMask | FC::Data_CFPoll): __LOG("QoS data + CF-poll") break;
							case (FC::QoSMask | FC::Data_CFAck_CFPoll): __LOG("QoS data + CF-ack + CF-poll") break;
							case (FC::QoSMask | FC::Null): __LOG("QoS version of the null frame") break;
							case (FC::QoSMask | FC::CFPoll): __LOG("QoS + CF-poll (no data)") break;
							case (FC::QoSMask | FC::CFAck): __LOG("QoS + CF-ack (no data)") break;
						}
						break;
				}

				Log::Flat("Info: Flags = | ");
				if (frame_ctrl.flags & FC::ToDS) Log::Flat("ToDS | ");
				if (frame_ctrl.flags & FC::FromDS) Log::Flat("FromDS | ");
				if (frame_ctrl.flags & FC::MoreFrag) Log::Flat("MoreFrag | ");
				if (frame_ctrl.flags & FC::Retry) Log::Flat("Retry | ");
				if (frame_ctrl.flags & FC::PowerMgmt) Log::Flat("PowerMgmt | ");
				if (frame_ctrl.flags & FC::MoreData) Log::Flat("MoreData | ");
				if (frame_ctrl.flags & FC::WEP) Log::Flat("WEP | ");
				if (frame_ctrl.flags & FC::Order) Log::Flat("Order | ");
				__LOG_NL()
				break;

			case FieldID_DurationID:
				break;

			case FieldID_Addr1:
				memcpy(addr_1, &frame[offset], FieldSize_Addr);
				__LOG("Address 1: %02x:%02x:%02x:%02x:%02x:%02x", addr_1[0], addr_1[1], addr_1[2], addr_1[3], addr_1[4], addr_1[5])
				break;

			case FieldID_Addr2:
				memcpy(addr_2, &frame[offset], FieldSize_Addr);
				__LOG("Address 2: %02x:%02x:%02x:%02x:%02x:%02x", addr_2[0], addr_2[1], addr_2[2], addr_2[3], addr_2[4], addr_2[5])
				break;

			case FieldID_Addr3:
				memcpy(addr_3, &frame[offset], FieldSize_Addr);
				__LOG("Address 3: %02x:%02x:%02x:%02x:%02x:%02x", addr_3[0], addr_3[1], addr_3[2], addr_3[3], addr_3[4], addr_3[5])
				break;

			case FieldID_Addr4:
				memcpy(addr_4, &frame[offset], FieldSize_Addr);
				__LOG("Address 4: %02x:%02x:%02x:%02x:%02x:%02x", addr_4[0], addr_4[1], addr_4[2], addr_4[3], addr_4[4], addr_4[5])
				break;

			case FieldID_SeqCtrl:
				memcpy(&seq_ctrl, &frame[offset], FieldSize_SeqCtrl);
				__LOG("Fragment number = %d", seq_ctrl.frag_num)
				__LOG("Sequence number = %d", seq_ctrl.seq_num)
				break;

			case FieldID_QoSControl:
				break;

			case FieldID_HTCtrl:
				break;
		}
		offset += f->bytes;
	}
}


