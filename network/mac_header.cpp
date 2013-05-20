	#include <string.h>
	#include "mac_header.h"
	#include "core/log.h"

	using namespace owl;

//-----------------------------------------------------------------------------
MACHeader::MACHeader(uchar* _frame)
	: frame(_frame)
	, fctrl(0)
	, duration_id(0)
	, seq_ctrl(0)
	, fcs(0)
{
	for (int i = 0; i < 6; ++i)
		addr_1[i] = addr_2[i] = addr_3[i] = addr_4[i] = 0;
	fctrl = (FrameControl*)frame;
}

//-----------------------------------------------------------------------------
void			MACHeader::BuildFrameFormat()
{
	typedef	FrameControl		FC;
	typedef	MACHeader::Field	MF;

	ff	<< new MF(FID_FrameCtrl, FS_FrameCtrl)
		<< new MF(FID_DurationID, FS_DurationID)
		<< new MF(FID_Addr1, FS_Addr);

	switch (fctrl->type)
	{
		case Management:
			ff	<< new MF(FID_Addr2, FS_Addr)
				<< new MF(FID_Addr3, FS_Addr)
				<< new MF(FID_SeqCtrl, FS_SeqCtrl);

			if (!(fctrl->subtype & FC::QoSMask) && (fctrl->flags & FC::Order))
			{
				// Frame contain an MSDU transfered using the StrictlyOrdered service class
			}
			else if ((fctrl->subtype & FC::QoSMask) && (fctrl->flags & FC::Order))
				ff << new MF(FID_HTCtrl, FS_HTCtrl);

			break;

		case Control:
			switch (fctrl->subtype)
			{
				case FC::BlockAckReq:
				case FC::BlockAck:
				case FC::PS_Poll:
				case FC::RTS:
				case FC::CFEnd:
				case FC::CFEnd_CFAck:
					ff << new MF(FID_Addr2, FS_Addr);
					break;
			}
			break;

		case Data:
			ff	<< new MF(FID_Addr2, FS_Addr)
				<< new MF(FID_Addr3, FS_Addr)
				<< new MF(FID_SeqCtrl, FS_SeqCtrl);

			if ((fctrl->flags & FC::ToDS) && (fctrl->flags & FC::FromDS))
				ff << new MF(FID_Addr4, FS_Addr);

			break;
	}
	ff << new MF(FID_FCS, FS_FCS);
}

//-----------------------------------------------------------------------------
void			MACHeader::Parse()
{
	typedef	FrameControl FC;

	if (fctrl->type == Management && fctrl->subtype == FC::Beacon)
		return;

	Log::NewLine();

	int offset = 0;
	for(int i = 0; i < ff.Size(); ++i)
	{
		MACHeader::Field* f = ff[i];
		switch (f->id)
		{
			case FID_FrameCtrl:
				Log::i("Frame control data:");
				Log::i("Version = %d", fctrl->version);

				switch (fctrl->type)
				{
					case Management:
						Log::i("Type: Management");
						switch (fctrl->subtype)
						{
							case FC::AssocRes: Log::i("Subtype: Association response"); break;
							case FC::ReassocReq: Log::i("Subtype: Reassociation request"); break;
							case FC::ReassocRes: Log::i("Subtype: Reassociation response"); break;
							case FC::ProbeReq: Log::i("Subtype: Probe request"); break;
							case FC::ProbeRes: Log::i("Subtype: Probe response"); break;
							case FC::Beacon: Log::i("Subtype: Beacon"); break;
							case FC::ATIM: Log::i("Subtype: ATIM"); break;
							case FC::Disassoc: Log::i("Subtype: Disassociation"); break;
							case FC::Auth: Log::i("Subtype: Authentication"); break;
							case FC::Deauth: Log::i("Subtype: Deauthentication"); break;
							case FC::Action: Log::i("Subtype: Action"); break;
						}
						break;

					case Control:
						Log::i("Type: Control");
						switch (fctrl->subtype)
						{
							case FC::BlockAckReq: Log::i("Subtype: Block acknowledgement request"); break;
							case FC::BlockAck: Log::i("Subtype: Block acknowledgement"); break;
							case FC::PS_Poll: Log::i("Subtype: PS-Poll"); break;
							case FC::RTS: Log::i("Subtype: Request-to-send frame (RTS)"); break;
							case FC::CTS: Log::i("Subtype: Clear-to-send frame (CTS)"); break;
							case FC::ACK: Log::i("Subtype: Acknowledge frame (ACK)"); break;
							case FC::CFEnd: Log::i("CF-end"); break;
							case FC::CFEnd_CFAck: Log::i("CF-end + CF-ack"); break;
						}
						break;

					case Data:
						Log::i("Type: Data");
						switch (fctrl->subtype)
						{
							case FC::Data: Log::i("Data frame"); break;
							case FC::Data_CFAck: Log::i("Data frame + CF-ack"); break;
							case FC::Data_CFPoll: Log::i("Data frame + CF-poll"); break;
							case FC::Data_CFAck_CFPoll: Log::i("Data frame + CF-ack + CF-poll"); break;
							case FC::Null: Log::i("Null frame"); break;
							case FC::CFAck: Log::i("CF-ack"); break;
							case FC::CFPoll: Log::i("CF-poll"); break;
							case FC::CFAck_CFPoll: Log::i("CF-ack + CF-poll"); break;
							case (FC::QoSMask | FC::Data): Log::i("QoS version of the data frame"); break;
							case (FC::QoSMask | FC::Data_CFAck): Log::i("QoS data + CF-ack"); break;
							case (FC::QoSMask | FC::Data_CFPoll): Log::i("QoS data + CF-poll"); break;
							case (FC::QoSMask | FC::Data_CFAck_CFPoll): Log::i("QoS data + CF-ack + CF-poll"); break;
							case (FC::QoSMask | FC::Null): Log::i("QoS version of the null frame"); break;
							case (FC::QoSMask | FC::CFPoll): Log::i("QoS + CF-poll (no data)"); break;
							case (FC::QoSMask | FC::CFAck): Log::i("QoS + CF-ack (no data)"); break;
						}
						break;
				}

				Log::Flat("Info: Flags = | ");
				if (fctrl->flags & FC::ToDS) Log::Flat("ToDS | ");
				if (fctrl->flags & FC::FromDS) Log::Flat("FromDS | ");
				if (fctrl->flags & FC::MoreFrag) Log::Flat("MoreFrag | ");
				if (fctrl->flags & FC::Retry) Log::Flat("Retry | ");
				if (fctrl->flags & FC::PowerMgmt) Log::Flat("PowerMgmt | ");
				if (fctrl->flags & FC::MoreData) Log::Flat("MoreData | ");
				if (fctrl->flags & FC::WEP) Log::Flat("WEP | ");
				if (fctrl->flags & FC::Order) Log::Flat("Order | ");
				Log::NewLine();
				break;

			case FID_DurationID:
				break;

			case FID_Addr1:
				memcpy(addr_1, &frame[offset], sizeof(uchar) * 6);
				Log::i("Address 1: %02x:%02x:%02x:%02x:%02x:%02x", addr_1[0], addr_1[1], addr_1[2], addr_1[3], addr_1[4], addr_1[5]);
				break;

			case FID_Addr2:
				memcpy(addr_2, &frame[offset], sizeof(uchar) * 6);
				Log::i("Address 2: %02x:%02x:%02x:%02x:%02x:%02x", addr_2[0], addr_2[1], addr_2[2], addr_2[3], addr_2[4], addr_2[5]);
				break;

			case FID_Addr3:
				memcpy(addr_3, &frame[offset], sizeof(uchar) * 6);
				Log::i("Address 3: %02x:%02x:%02x:%02x:%02x:%02x", addr_3[0], addr_3[1], addr_3[2], addr_3[3], addr_3[4], addr_3[5]);
				break;

			case FID_Addr4:
				memcpy(addr_4, &frame[offset], sizeof(uchar) * 6);
				Log::i("Address 4: %02x:%02x:%02x:%02x:%02x:%02x", addr_4[0], addr_4[1], addr_4[2], addr_4[3], addr_4[4], addr_4[5]);
				break;

			case FID_SeqCtrl:
				break;

			case FID_QoSControl:
				break;

			case FID_HTCtrl:
				break;
		}
		offset += f->bytes;
	}
}


