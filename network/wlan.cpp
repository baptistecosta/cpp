	#include <windows.h>
	#include <wlanapi.h>
	#include <stdio.h>
	#include <string>
	#include <time.h>
	#include "wlan.h"
	#include "core/time.h"
	#include "core/log.h"

	using namespace owl;

//---------------------------------------------------------------------------
BWLAN::BWLAN(const char* log_file_path)
	: handle(INVALID_HANDLE_VALUE)
	, version(0)
	, f(NULL)
{
	fopen_s(&f, log_file_path, "w");
	if (!f)
	{
		fprintf(stderr, "ERROR: Failed to open file %s", log_file_path);
		exit(-1);
	}

	if (int res = WlanOpenHandle(1, 0, &version, &handle) != ERROR_SUCCESS)
	{
		printf("ERROR: WlanEnumInterfaces %d", res);
		exit(-1);
	}
}

//---------------------------------------------------------------------------
BWLAN::~BWLAN()
{	WlanCloseHandle(handle, NULL);	}

//---------------------------------------------------------------------------
PWLAN_INTERFACE_INFO_LIST BWLAN::GetInterfaceInfoList()
{
	PWLAN_INTERFACE_INFO_LIST if_info_list = NULL;
	if (int res = WlanEnumInterfaces(handle, 0, &if_info_list) != ERROR_SUCCESS)
	{
		printf("ERROR: WlanEnumInterfaces %d", res);
		CloseHandle(handle);
		exit(-1);
	}
	return if_info_list;
}

//---------------------------------------------------------------------------
void			BWLAN::Scan(GUID& if_guid)
{
	if (int res = WlanScan(handle, &if_guid, 0, 0, 0) != ERROR_SUCCESS)
	{
		printf("ERROR: WlanScan %d", res);
		CloseHandle(handle);
		exit(-1);
	}
	// Wait until scan completes
	Sleep(2000);
}

//---------------------------------------------------------------------------
PWLAN_BSS_LIST	BWLAN::EnumInterfaces(GUID& if_guid)
{
	PWLAN_BSS_LIST bss_list = NULL;
	if (int res = WlanGetNetworkBssList(handle, &if_guid, NULL, dot11_BSS_type_any, NULL, NULL, &bss_list) != ERROR_SUCCESS)
	{
		printf("ERROR: WlanGetNetworkBssList %d", res);
		CloseHandle(handle);
		exit(-1);
	}
	return bss_list;
}

//---------------------------------------------------------------------------
void			BWLAN::DumpInterfaceInfoList(PWLAN_INTERFACE_INFO_LIST if_info_list)
{
	Log::Tee(f, "*********************** Interfaces info list ****************************\n");
	for (unsigned int i = 0; i < if_info_list->dwNumberOfItems; i++)
	{
		WLAN_INTERFACE_INFO& if_info = if_info_list->InterfaceInfo[if_info_list->dwIndex];
		WCHAR guid[256] = {0};
		StringFromGUID2(if_info.InterfaceGuid, guid, 256);
		Log::Tee(f, "%d- %ws", i + 1, if_info.strInterfaceDescription);
		Log::Tee(f, " | GUID: %ws\n", guid);

		Log::Tee(f, "  State:\t");
		switch (if_info.isState)
		{
			case wlan_interface_state_not_ready: Log::Tee(f, "Not ready\n"); break;
			case wlan_interface_state_connected: Log::Tee(f, "Connected\n"); break;
			case wlan_interface_state_ad_hoc_network_formed: Log::Tee(f, "First node in a ad hoc network\n"); break;
			case wlan_interface_state_disconnecting: Log::Tee(f, "Disconnecting\n"); break;
			case wlan_interface_state_disconnected: Log::Tee(f, "Not connected\n"); break;
			case wlan_interface_state_associating: Log::Tee(f, "Attempting to associate with a network\n"); break;
			case wlan_interface_state_discovering: Log::Tee(f, "Auto configuration is discovering settings for the network\n"); break;
			case wlan_interface_state_authenticating: Log::Tee(f, "In process of authenticating\n"); break;
			default:
				Log::Tee(f, "Unknown state %ld\n", if_info.isState);
				break;
		}
	}
	Log::Tee(f, "\n");
}

//---------------------------------------------------------------------------
void			BWLAN::QueryInterface(GUID& if_guid)
{
	DWORD connect_info_size = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	PWLAN_CONNECTION_ATTRIBUTES connect_info = NULL;
	WLAN_OPCODE_VALUE_TYPE op_code = wlan_opcode_value_type_invalid;

	int res = WlanQueryInterface(handle, &if_guid, wlan_intf_opcode_current_connection, NULL, &connect_info_size, (PVOID*)&connect_info, &op_code);

	if (res != ERROR_SUCCESS)
	{
		Log::Tee(f, "ERROR: WlanQueryInterface failed: %u\n", res);
		exit(-1);
	}
	Log::Tee(f, "WLAN_CONNECTION_ATTRIBUTES for this interface\n");
	Log::Tee(f, "Interface State:\t ");
	switch (connect_info->isState)
	{
		case wlan_interface_state_not_ready: Log::Tee(f, "Not ready\n"); break;
		case wlan_interface_state_connected: Log::Tee(f, "Connected\n"); break;
		case wlan_interface_state_ad_hoc_network_formed: Log::Tee(f, "First node in a ad hoc network\n"); break;
		case wlan_interface_state_disconnecting: Log::Tee(f, "Disconnecting\n"); break;
		case wlan_interface_state_disconnected: Log::Tee(f, "Not connected\n"); break;
		case wlan_interface_state_associating: Log::Tee(f, "Attempting to associate with a network\n"); break;
		case wlan_interface_state_discovering: Log::Tee(f, "Auto configuration is discovering settings for the network\n"); break;
		case wlan_interface_state_authenticating: Log::Tee(f, "In process of authenticating\n"); break;
		default: Log::Tee(f, "Unknown state %ld\n", connect_info->isState); break;
	}

	Log::Tee(f, "Connection Mode:\t ");
	switch (connect_info->wlanConnectionMode)
	{
		case wlan_connection_mode_profile: Log::Tee(f, "A profile is used to make the connection\n"); break;
		case wlan_connection_mode_temporary_profile: Log::Tee(f, "A temporary profile is used to make the connection\n"); break;
		case wlan_connection_mode_discovery_secure: Log::Tee(f, "Secure discovery is used to make the connection\n"); break;
		case wlan_connection_mode_discovery_unsecure: Log::Tee(f, "Unsecure discovery is used to make the connection\n"); break;
		case wlan_connection_mode_auto: Log::Tee(f, "connection initiated by wireless service automatically using a persistent profile\n"); break;
		case wlan_connection_mode_invalid: Log::Tee(f, "Invalid connection mode\n"); break;
		default: Log::Tee(f, "Unknown connection mode %ld\n", connect_info->wlanConnectionMode); break;
	}

	Log::Tee(f, "Profile name used:\t %ws\n", connect_info->strProfileName);
	Log::Tee(f, "Association Attributes for this connection\n");
	Log::Tee(f, "  SSID:                      ");

	if (connect_info->wlanAssociationAttributes.dot11Ssid.uSSIDLength == 0)
		Log::Tee(f, "\n");
	else
	{
		for (unsigned int k = 0; k < connect_info->wlanAssociationAttributes.dot11Ssid.uSSIDLength; k++)
			Log::Tee(f, "%c", (int)connect_info->wlanAssociationAttributes.dot11Ssid.ucSSID[k]);
		Log::Tee(f, "\n");
	}

	Log::Tee(f, "  BSS Network type:          ");
	switch (connect_info->wlanAssociationAttributes.dot11BssType)
	{
		case dot11_BSS_type_infrastructure: Log::Tee(f, "Infrastructure\n"); break;
		case dot11_BSS_type_independent: Log::Tee(f, "Infrastructure\n"); break;
		default: Log::Tee(f, "Other = %lu\n", connect_info->wlanAssociationAttributes.dot11BssType); break;
	}

	Log::Tee(f, "  MAC address:               ");
	for (int k = 0; k < sizeof (connect_info->wlanAssociationAttributes.dot11Bssid); k++)
	{
		if (k == 5)	Log::Tee(f, "%.2X\n", connect_info->wlanAssociationAttributes.dot11Bssid[k]);
		else		Log::Tee(f, "%.2X:", connect_info->wlanAssociationAttributes.dot11Bssid[k]);
	}

	Log::Tee(f, "  PHY network type:          ");
	switch (connect_info->wlanAssociationAttributes.dot11PhyType)
	{
		case dot11_phy_type_fhss: Log::Tee(f, "Frequency-hopping spread-spectrum (FHSS)\n"); break;
		case dot11_phy_type_dsss: Log::Tee(f, "Direct sequence spread spectrum (DSSS)\n"); break;
		case dot11_phy_type_irbaseband: Log::Tee(f, "Infrared (IR) baseband\n"); break;
		case dot11_phy_type_ofdm: Log::Tee(f, "Orthogonal frequency division multiplexing (OFDM)\n"); break;
		case dot11_phy_type_hrdsss: Log::Tee(f, "High-rate DSSS (HRDSSS) = \n"); break;
		case dot11_phy_type_erp: Log::Tee(f, "Extended rate PHY type\n"); break;
		case dot11_phy_type_ht: Log::Tee(f, "802.11n PHY type\n"); break;
		default: Log::Tee(f, "Unknown = %lu\n", connect_info->wlanAssociationAttributes.dot11PhyType); break;
	}

	Log::Tee(f, "  PHY index:                 %u\n", connect_info->wlanAssociationAttributes.uDot11PhyIndex);
	Log::Tee(f, "  Signal Quality:            %d\n", connect_info->wlanAssociationAttributes.wlanSignalQuality);
	Log::Tee(f, "  Receiving Rate:            %ld\n", connect_info->wlanAssociationAttributes.ulRxRate);
	Log::Tee(f, "  Transmission Rate:         %ld\n", connect_info->wlanAssociationAttributes.ulTxRate);

	Log::Tee(f, "Security Attributes for this connection\n");
	Log::Tee(f, "  Security enabled:          ");
	Log::Tee(f, connect_info->wlanSecurityAttributes.bSecurityEnabled == 0 ? "No\n" : "Yes\n");
	Log::Tee(f, "  802.1X enabled:            ");
	Log::Tee(f, connect_info->wlanSecurityAttributes.bOneXEnabled == 0 ? "No\n" : "Yes\n");
	Log::Tee(f, "  Authentication Algorithm:  ");

	switch (connect_info->wlanSecurityAttributes.dot11AuthAlgorithm)
	{
		case DOT11_AUTH_ALGO_80211_OPEN: Log::Tee(f, "802.11 Open\n"); break;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY: Log::Tee(f, "802.11 Shared\n"); break;
		case DOT11_AUTH_ALGO_WPA: Log::Tee(f, "WPA\n"); break;
		case DOT11_AUTH_ALGO_WPA_PSK: Log::Tee(f, "WPA-PSK\n"); break;
		case DOT11_AUTH_ALGO_WPA_NONE: Log::Tee(f, "WPA-None\n"); break;
		case DOT11_AUTH_ALGO_RSNA: Log::Tee(f, "RSNA\n"); break;
		case DOT11_AUTH_ALGO_RSNA_PSK: Log::Tee(f, "RSNA with PSK\n"); break;
		default: Log::Tee(f, "Other (%lu)\n", connect_info->wlanSecurityAttributes.dot11AuthAlgorithm); break;
	}

	Log::Tee(f, "  Cipher Algorithm:          ");
	switch (connect_info->wlanSecurityAttributes.dot11CipherAlgorithm)
	{
		case DOT11_CIPHER_ALGO_NONE: Log::Tee(f, "None\n"); break;
		case DOT11_CIPHER_ALGO_WEP40: Log::Tee(f, "WEP-40\n"); break;
		case DOT11_CIPHER_ALGO_TKIP: Log::Tee(f, "TKIP\n"); break;
		case DOT11_CIPHER_ALGO_CCMP: Log::Tee(f, "CCMP\n"); break;
		case DOT11_CIPHER_ALGO_WEP104: Log::Tee(f, "WEP-104\n"); break;
		case DOT11_CIPHER_ALGO_WEP: Log::Tee(f, "WEP\n"); break;
		default: Log::Tee(f, "Other (0x%x)\n", connect_info->wlanSecurityAttributes.dot11CipherAlgorithm); break;
	}
	Log::Tee(f, "\n");

	if (connect_info != NULL)
	{
		WlanFreeMemory(connect_info);
		connect_info = NULL;
	}
}


//---------------------------------------------------------------------------
void			BWLAN::DumpBSSList(PWLAN_BSS_LIST bss_list)
{
	Log::Tee(f, "*************************************************************************\n");
	Log::Tee(f, "Basic Server Set (BSS) found on air:\n\n");
	for (unsigned int i = 0; i < bss_list->dwNumberOfItems; i++)
	{
		WLAN_BSS_ENTRY& bss = bss_list->wlanBssEntries[i];

		WCHAR* bss_type = L"";
		if (bss.dot11BssType == 1)
			bss_type = L"Infrastructure BSS network";
		else
			bss_type = L"Independent BSS (IBSS) network (ad hoc network)";

		WCHAR* phy_type = L"";
		if		(bss.dot11BssPhyType == dot11_phy_type_unknown) phy_type = L"PHY type unknown";
		else if (bss.dot11BssPhyType == dot11_phy_type_fhss) phy_type = L"Frequency-hopping spread-spectrum - FHSS (Bluetooth)";
		else if (bss.dot11BssPhyType == dot11_phy_type_dsss) phy_type = L"Direct sequence spread spectrum - DSSS";
		else if (bss.dot11BssPhyType == dot11_phy_type_irbaseband) phy_type = L"Infrared baseband - IR";
		else if (bss.dot11BssPhyType == dot11_phy_type_ofdm) phy_type = L"Orthogonal frequency division multiplexing (802.11a)";
		else if (bss.dot11BssPhyType == dot11_phy_type_hrdsss) phy_type = L"High-rate DSSS - HRDSSS";
		else if (bss.dot11BssPhyType == dot11_phy_type_erp) phy_type = L"Extended rate PHY - ERP (802.11g)";
		else if (bss.dot11BssPhyType == dot11_phy_type_ht) phy_type = L"802.11n";
		else if (bss.dot11BssPhyType == dot11_phy_type_IHV_start) phy_type = L"IHV Start";
		else if (bss.dot11BssPhyType == dot11_phy_type_IHV_end) phy_type = L"IHV End";
		else
			phy_type = L"PHY type unknown";

		char ts[24] = {0};
		TimeToDate(bss.ullTimestamp, ts);

		char host_ts[24] = {0};
		TimeToDate(bss.ullHostTimestamp, host_ts);

		u_short flags = bss.usCapabilityInformation;
		std::string cap_info = "|";
		if (flags & 0x1) cap_info += "Extended Service Set|";
		if (flags & 0x2) cap_info += "Independent Basic Service Set|";
		if (flags & 0x4) cap_info += "Pollable|";
		if (flags & 0x8) cap_info += "Handles poll requests|";
		if (flags & 0x16) cap_info += "Encryption required|";

		void* beacon_frame = malloc(bss.ulIeSize);
		ULONG bss_ie_blob_begin = bss.ulIeOffset;
		PBYTE blob = (PBYTE)&bss + bss_ie_blob_begin;
		memcpy((void*)beacon_frame, (void*)blob, bss.ulIeSize);

		Log::Tee(f, "//-----------------------------------------------------------------------\n");
		Log::Tee(f, "// #%d\n", i + 1);
		Log::Tee(f, "SSID:                                       %s\n", bss.dot11Ssid.ucSSID);
		Log::Tee(f, "OSI Physical Layer ID (PHY):                %ld\n", bss.uPhyId);
		Log::Tee(f, "BSSID (MAC address of AP):                  %02x:%02x:%02x:%02x:%02x:%02x\n", bss.dot11Bssid[0], bss.dot11Bssid[1], bss.dot11Bssid[2], bss.dot11Bssid[3], bss.dot11Bssid[4], bss.dot11Bssid[5]);
		Log::Tee(f, "BSS Type:                                   %ws\n", bss_type);
		Log::Tee(f, "BSS PHY Type:                               %ws\n", phy_type);
		Log::Tee(f, "Received signal strength indicator (RSSI):  %ld dBm\n", bss.lRssi);
		Log::Tee(f, "Link quality:                               %ld\n", bss.uLinkQuality);
		Log::Tee(f, "Beacon period:                              %d\n", bss.usBeaconPeriod);
		Log::Tee(f, "Timestamp:                                  %s\n", ts);
		Log::Tee(f, "Host timestamp:                             %s\n", host_ts);
		Log::Tee(f, "Capability information:                     %d %s\n", bss.usCapabilityInformation, cap_info.c_str());
		Log::Tee(f, "Channel Center Frequency:                   %ld kHz\n", bss.ulChCenterFrequency);
		Log::Tee(f, "Offset in bytes of IE data blob:            %ld\n", bss.ulIeOffset);
		Log::Tee(f, "Size in bytes of IE data blob:              %ld\n", bss.ulIeSize);
		Log::Tee(f, "\n");
		Log::Hex((const u_char*)beacon_frame, bss.ulIeSize, f);
		Log::Tee(f, "\n");

		free(beacon_frame);
	}
	Log::Tee(f, "\n");
}
