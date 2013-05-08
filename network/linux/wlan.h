#ifndef __WIRELESS__
#define __WIRELESS__

	#include <vector>
	#include "externs/wireless_tools/iwlib.h"

//!
struct AccessPoint
{
	char			bssid[18],
					essid[IW_ESSID_MAX_SIZE + 1];
	int 			signal_level,
					signal_noise;

	AccessPoint();

	void			Log();
};

//!
class WifiSniffer
{
	const char*		iface;
	int 			sock;
	iw_range 		range;

public:

	WifiSniffer(const char* ifname);
	~WifiSniffer();

	void			Init();
	bool			Scan(std::vector<AccessPoint>& sniff);
};

#endif	//	__WIRELESS__
