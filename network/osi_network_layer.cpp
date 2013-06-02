/**
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

	#include <string.h>
	#include "core/containers/vector.h"
	#include "osi_network_layer.h"
	#include "utils.h"
	
	using namespace owl;

//---------------------------------------------------------------------------
EAPOL::EAPOL(const ui8* f)
{
	for (int i = 0; i < EAPOL::Size; ++i)
		frame[i] = f[i];

	int offset = 0;
	version = NetworkUtils::FromRawFrame<ui8>(&f[offset]);
	offset += Version_Size;
	pck_type = NetworkUtils::FromRawFrame<ui8>(&f[offset]);
	offset += PacketType_Size;
	pck_body_len = NetworkUtils::FromRawFrame<ui16>(&f[offset]);
	offset += PacketBodyLenght_Size;
	descriptor_type = NetworkUtils::FromRawFrame<ui8>(&f[offset]);
	offset += DescriptorType_Size;
	key_info = NetworkUtils::FromRawFrame<ui16>(&f[offset]);
	offset += KeyInfo_Size;
	key_len = NetworkUtils::FromRawFrame<ui16>(&f[offset]);
	offset += KeyLen_Size;
	replay_counter = NetworkUtils::FromRawFrame<ui64>(&f[offset]);
	offset += KeyReplayCounter_Size;

	memcpy(&key_nounce, &f[offset], KeyNonce_Size);
	offset += KeyNonce_Size;
	memcpy(&key_iv, &f[offset], KeyIV_Size);
	offset += KeyIV_Size;

	key_rsc = NetworkUtils::FromRawFrame<ui64>(&f[offset]);
	offset += KeyRSC_Size;
	key_id = NetworkUtils::FromRawFrame<ui64>(&f[offset]);
	offset += KeyID_Size;

	// Copy MIC
	memcpy(&key_mic, &f[offset], KeyMIC_Size);
	// Replace MIC by '\0' in raw frame
	memset(&frame[offset], 0, KeyMIC_Size);
	offset += KeyMIC_Size;

	key_data_len = NetworkUtils::FromRawFrame<ui16>(&f[offset]);
	offset += KeyDataLength_Size;
}
