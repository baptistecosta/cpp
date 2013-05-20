#ifndef __WIRELESS__
#define __WIRELESS__

	#include <vector>
	#include "externs/wireless_tools/iwlib.h"

class	Interface
{
		int				sockfd;
		const char*		ifname;

public:

		Interface(const char* ifname);
		~Interface();

		int				GetIndex();
		short int		GetFlags();
		sockaddr		GetHardwareAddress();

		short int		SetFlags(short int value);
		bool			SetToMonitorMode();

		bool			Bind(const sockaddr*);

		void			LogFlagsInfo(short int flags);

		int				GetSocketFileDescriptor() const		{	return sockfd;	}
		const char*		GetInterfaceName() const			{	return ifname;	}
};

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
