	#include <stdio.h>
	#include "wlan.h"
	#include "core/log.h"

	using namespace owl;
	using namespace std;

//-----------------------------------------------------------------------------
AccessPoint::AccessPoint() : signal_level(0), signal_noise(0)
{
	memset(bssid, 0, sizeof(bssid));
	memset(essid, 0, sizeof(essid));
}

//-----------------------------------------------------------------------------
void			AccessPoint::Log()
{	Log::i("ESSID:  %s  BSSID:  %s  Signal level:  %d  Signal noise:  %d", essid, bssid, signal_level, signal_noise);	}

//-----------------------------------------------------------------------------
WifiSniffer::WifiSniffer(const char* ifname)
	: sock(-1)
	, iface(ifname)
{
	Init();
}

//-----------------------------------------------------------------------------
WifiSniffer::~WifiSniffer()
{
  if (sock >= 0)
    iw_sockets_close(sock);
}

//-----------------------------------------------------------------------------
void 			WifiSniffer::Init()
{
	sock = iw_sockets_open();
	if (sock < 0)
	{
		printf("couldn't open network socket on %s", iface);
		exit(-1);
	}
	if (iw_get_range_info(sock, iface, &range) < 0)
	{
		printf("interface %s doesn't support scanning\n", iface);
		exit(-1);
	}
}

//-----------------------------------------------------------------------------
bool			WifiSniffer::Scan(vector<AccessPoint>& sniffs)
{
	sniffs.clear();

	iwreq wrq;
	wrq.u.data.pointer = NULL;
	wrq.u.data.length = 0;

	// Initiate scanning
	if (iw_set_ext(sock, iface, SIOCSIWSCAN, &wrq) < 0)
	{
		if (errno == EBUSY)
			Log::i("Device busy");
		else if (errno == EPERM)
			Log::i("Couldn't start a scan, insufficient permissions?");
		else
			Log::i("Interface %s doesn't support scanning?: %s", iface, strerror(errno));
		return false;
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 250000;

	// Bail if it takes longer than 10 seconds
	int timeout = 10000000;
	timeout -= tv.tv_usec;

	uint8_t* scan_buf = NULL;
	int scan_buflen = IW_SCAN_MAX_DATA;
	for (bool scan_complete = false; !scan_complete; )
	{
		fd_set fds;
		FD_ZERO(&fds);
		int last_fd = -1;

		// Programs that want to continue running while also reading interactive
		// user input (or some other input that arrives unpredictably) need to be
		// multithreaded or they need to read input streams carefully and, specifically, conditionally.
		// select() can be used to implement the second design pattern.
		// It can determine whether input can be read without blocking the entire application.
		int ret = select(last_fd + 1, &fds, NULL, NULL, &tv);
		if (ret < 0)
		{
			if (errno == EAGAIN || errno == EINTR)
				continue;

			Log::i("Unhandled signal...");
			exit(-1);
		}

		if (ret == 0)
		{
			uint8_t* scan_newbuf = NULL;
			while (!scan_complete)
			{
				scan_newbuf = (uint8_t*)realloc(scan_buf, scan_buflen);
				if (scan_newbuf == NULL)
				{
					if (scan_buf)
						free(scan_buf);
					printf("malloc fail. system over.\n");
					return false;
				}
				scan_buf = scan_newbuf;

				wrq.u.data.pointer = scan_buf;
				wrq.u.data.flags = IW_SCAN_ALL_ESSID | IW_SCAN_ALL_FREQ | IW_SCAN_ALL_MODE | IW_SCAN_ALL_RATE;
				wrq.u.data.length = scan_buflen;
				if (iw_get_ext(sock, iface, SIOCGIWSCAN, &wrq) < 0)
				{
					if (errno == E2BIG && range.we_version_compiled > 16)
					{
						if (wrq.u.data.length > IW_SCAN_MAX_DATA)
							scan_buflen = wrq.u.data.length;
						else
							scan_buflen *= 2;
						Log::i("Grow buffer\n");
						continue;
					}
					else if (errno == EAGAIN)
					{
						tv.tv_sec = 0;
						tv.tv_usec = 250000;
						timeout -= tv.tv_usec;
						if (timeout < 0)
						{
							scan_complete = true;
							printf("hit scan timeout");
						}
						break;
					}
					// if we get here, something bad has happened
					free(scan_buf);
					printf("failed to read scan data");
					return false;
				}
				else
					scan_complete = true;
			}
		}
	}

	if (wrq.u.data.length)
	{
		AccessPoint sniff;
		iw_event iwe;
		stream_descr stream;
		iw_init_event_stream(&stream, (char*)scan_buf, wrq.u.data.length);
		int ret = 0;
		do
		{
			ret = iw_extract_event_stream(&stream, &iwe, range.we_version_compiled);
			if (ret > 0)
			{
				switch (iwe.cmd)
				{
					// Get access point MAC addresses
					case SIOCGIWAP:
						iw_ether_ntop((const ether_addr*)&iwe.u.ap_addr.sa_data, sniff.bssid);
						break;

					// Quality part of statistics (scan)
					case IWEVQUAL:
						if (iwe.u.qual.level != 0 || (iwe.u.qual.updated & (IW_QUAL_DBM | IW_QUAL_RCPI)))
						{
							sniff.signal_level = iwe.u.qual.level;
							sniff.signal_noise = iwe.u.qual.noise;
						}
						break;

					// Driver specific ascii string
					case IWEVCUSTOM:
						if (iwe.u.data.pointer && iwe.u.data.length)
						{
							char buf[IW_CUSTOM_MAX + 1] = {0};
							memcpy(buf, iwe.u.data.pointer, iwe.u.data.length);
							buf[iwe.u.data.length] = '\0';
							Log::i("Driver specific: [%s]", buf);
						}
						break;

					case SIOCGIWESSID:
						if (iwe.u.essid.pointer && iwe.u.essid.length)
							memcpy(sniff.essid, iwe.u.essid.pointer, iwe.u.essid.length);
						// This event seems to always happen last.
						// I hope this isn't just due to the wifi driver
						sniffs.push_back(sniff);
						break;
				}
			}
		} while (ret > 0);

		Log::i("Sniffed %d AP", sniffs.size());
		for (uint i = 0; i < sniffs.size(); i++)
			sniffs[i].Log();
	}
	else
		printf("no scan results");

	free(scan_buf);
	return true;
}
