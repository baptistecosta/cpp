	#include "oradiotap.h"
	#include "core/log.h"
	#include "defines.h"

	using namespace owl;

//-----------------------------------------------------------------------------
Radiotap::Radiotap(uchar* frame)
{
	ref_entries << new Entry(Radiotap::TSFT, 8)
		<< new Entry(Radiotap::FLAGS, 1)
		<< new Entry(Radiotap::RATE, 1)
		<< new Entry(Radiotap::CHANNEL, 4)
		<< new Entry(Radiotap::FHSS, 2)
		<< new Entry(Radiotap::DBM_ANTSIGNAL, 1)
		<< new Entry(Radiotap::DBM_ANTNOISE, 1)
		<< new Entry(Radiotap::LOCK_QUALITY, 2)
		<< new Entry(Radiotap::TX_ATTENUATION, 2)
		<< new Entry(Radiotap::DB_TX_ATTENUATION, 2)
		<< new Entry(Radiotap::DBM_TX_POWER, 1)
		<< new Entry(Radiotap::ANTENNA, 1)
		<< new Entry(Radiotap::DB_ANTSIGNAL, 1)
		<< new Entry(Radiotap::DB_ANTNOISE, 1)
		<< new Entry(Radiotap::RX_FLAGS, 2)
		<< new Entry(Radiotap::TX_FLAGS, 2)
		<< new Entry(Radiotap::RTS_RETRIES, 1)
		<< new Entry(Radiotap::DATA_RETRIES, 1)
		<< new Entry(Radiotap::MCS, 3)
		<< new Entry(Radiotap::AMPDU_STATUS, 8)
		<< new Entry(Radiotap::RADIOTAP_NAMESPACE, 1)
		<< new Entry(Radiotap::VENDOR_NAMESPACE, 1)
		<< new Entry(Radiotap::EXT, 1);

	int hdr_version_s = sizeof(hdr.version),
		hdr_pad_s = sizeof(hdr.pad),
		hdr_len_s = sizeof(hdr.len),
		hdr_present_s = sizeof(hdr.present);

	memcpy(&hdr.version, frame, hdr_version_s);
	memcpy(&hdr.len, &frame[hdr_version_s + hdr_pad_s], hdr_len_s);
	memcpy(&hdr.present, &frame[hdr_version_s + hdr_pad_s + hdr_len_s], hdr_present_s);

	Log(frame);
}

//-----------------------------------------------------------------------------
void			Radiotap::Log(uchar* frame)
{
	int data_len = 0;
	int offset = hdr.Len();

	for (int i = 0; i < ref_entries.Size(); ++i)
	{
		Entry* e = ref_entries[i];
		if (hdr.present & e->flag)
		{
			data_len += e->size;

			switch (e->flag)
			{
				case TSFT:
				{
					ullong* ts = (ullong*)&frame[offset];
					__LOG("TSTF: %ld", *ts)
					break;
				}
				case FLAGS:
				{
					uchar f = frame[offset];
					if (f & FLAGS_CFP)		__LOG("FLAGS: sent/received during CFP")
					if (f & FLAGS_SHORTPRE)	__LOG("FLAGS: sent/received with short preamble")
					if (f & FLAGS_WEP)		__LOG("FLAGS: sent/received with WEP encryption")
					if (f & FLAGS_FRAG)		__LOG("FLAGS: sent/received with fragmentation")
					if (f & FLAGS_FCS)		__LOG("FLAGS: frame includes FCS")
					if (f & FLAGS_DATAPAD)	__LOG("FLAGS: frame has padding between 802.11 header and payload (to 32-bit boundary)")
					if (f & FLAGS_BADFCS)	__LOG("FLAGS: frame failed FCS check")
					if (f & FLAGS_SGI)		__LOG("FLAGS: frame used short guard interval (HT)")
					break;
				}
				case RATE:
				{
					uchar b = frame[offset];
					int r = (int)b * 500;
					__LOG("TX/RX data rate = %d Kbps", r)
					break;
				}
				case CHANNEL:
				{
					ushort* freq = (ushort*)&frame[offset];
					__LOG("Channel frequency = %d MHz", *freq)

					ushort* flags = (ushort*)&frame[offset + sizeof(ushort)];
					Log::Flat("Info: Channel flags: | ");
					if (*flags & CHANNEL_TURBO)		Log::Flat("Turbo channel | ");
					if (*flags & CHANNEL_CCK)		Log::Flat("Complementary Code Keying (CCK) | ");
					if (*flags & CHANNEL_OFDM)		Log::Flat("OFDM channel | ");
					if (*flags & CHANNEL_2GHZ)		Log::Flat("2 GHz spectrum channel |");
					if (*flags & CHANNEL_5GHZ)		Log::Flat("5 GHz spectrum channel |");
					if (*flags & CHANNEL_PASSIVE)	Log::Flat("Only passive scan allowed |");
					if (*flags & CHANNEL_DYN)		Log::Flat("Dynamic CCK-OFDM channel |");
					if (*flags & CHANNEL_GFSK)		Log::Flat("GFSK channel (FHSS PHY) |");
					Log::NewLine();
					break;
				}
				case FHSS:
					__LOG("FHSS hop set: %x", frame[offset])
					__LOG("FHSS hop pattern: %x", frame[offset + sizeof(uchar)])
					break;
				case DBM_ANTSIGNAL:
					__LOG("Antenna signal = %d dBm", frame[offset])
					break;
				case DBM_ANTNOISE:
					__LOG("Antenna noise = %d dBm", frame[offset])
					break;
				case LOCK_QUALITY:
				{
					ushort* b = (ushort*)&frame[offset];
					__LOG("Lock quality = %d", *b)
					break;
				}
				case TX_ATTENUATION:
				{
					ushort* b = (ushort*)&frame[offset];
					__LOG("TX attenuation = %d", *b)
					break;
				}
				case DB_TX_ATTENUATION:
				{
					ushort* b = (ushort*)&frame[offset];
					__LOG("dB TX attenuation = %d", *b)
					break;
				}
				case DBM_TX_POWER:
					__LOG("dBm TX power = %d", frame[offset])
					break;
				case ANTENNA:
					__LOG("Antenna index = %d", frame[offset])
					break;
				case DB_ANTSIGNAL:
					__LOG("dB antenna signal = %d", frame[offset])
					break;
				case DB_ANTNOISE:
					__LOG("dB antenna noise = %d", frame[offset])
					break;
				case RX_FLAGS:
				{
					ushort* flags = (ushort*)&frame[offset];
					if (*flags & FLAG_RX_BADPLCP)
						__LOG("PLCP CRC check failed")
					break;
				}
				case MCS:
				{
					char known = (char)frame[offset];
					if (known & MCS_HAVE_BW)	__LOG("MCS known: bandwidth")
					if (known & MCS_HAVE_MCS)	__LOG("MCS known: MCS index")
					if (known & MCS_HAVE_GI)	__LOG("MCS known: guard interval")
					if (known & MCS_HAVE_FMT)	__LOG("MCS known: HT format")
					if (known & MCS_HAVE_FEC)	__LOG("MCS known: FEC type")

					{
					uchar flag = frame[offset + sizeof(uchar)];
					if		(flag == MCS_BW_20)		__LOG("MCS bandwidth = 20")
					else if (flag == MCS_BW_40)		__LOG("MCS bandwidth = 40")
					else if (flag == MCS_BW_20L)	__LOG("MCS bandwidth = 20L")
					else if (flag == MCS_BW_20U)	__LOG("MCS bandwidth = 20U")
					}

					uchar mcs = frame[offset + sizeof(uchar) * 2];
					__LOG("MCS rate index = %d", mcs)
					break;
				}
			}
			offset += e->size;
		}
	}
	__LOG("Radiotap segment length = %d", hdr.Len() + data_len)
}
