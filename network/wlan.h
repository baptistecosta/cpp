#ifndef __NETWORK_WLAN__
#define __NETWORK_WLAN__

namespace owl
{

struct BWLAN
{
	HANDLE			handle;
	DWORD			version;

	FILE*			f;

	BWLAN(const char* log_file_path = "log.txt");
	~BWLAN();

	PWLAN_INTERFACE_INFO_LIST GetInterfaceInfoList();
	void			Scan(GUID&);
	PWLAN_BSS_LIST	EnumInterfaces(GUID&);
	void			QueryInterface(GUID& if_guid);

	void			DumpInterfaceInfoList(PWLAN_INTERFACE_INFO_LIST);
	void			DumpBSSList(PWLAN_BSS_LIST);
};

}		//	owl
#endif	//	__NETWORK_WLAN__