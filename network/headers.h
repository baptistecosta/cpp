#ifndef __NETWORK_HEADERS__
#define __NETWORK_HEADERS__

namespace owl {

// struct IPAddress
// {
// 	u_char			byte1,
// 					byte2,
// 					byte3,
// 					byte4;
// };

// struct IPHeader
// {
// 	u_char			ver_ihl,		// Version (4 bits) + Internet header length (4 bits)
// 					tos;			// Type of service 
// 	u_short			tlen,			// Total length 
// 					identification,	// Identification
// 					flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
// 	u_char			ttl,			// Time to live
// 					proto;			// Protocol
// 	u_short			crc;			// Header checksum
// 	IPAddress		saddr,			// Source address
// 					daddr;			// Destination address
// 	u_int			op_pad;			// Option + Padding
// };

// struct UDPHeader
// {
// 	u_short			sport,			// Source port
// 					dport,			// Destination port
// 					len,			// Datagram length
// 					crc;			// Checksum
// };


struct EthernetHeader
{
	u_char			dest[6],
					source[6];
	u_short			type;
};

struct IPv4Header
{
	u_char			ip_header_len :4,	// 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
					ip_version :4,		// 4-bit IPv4 version
					ip_tos;				// IP type of service
	u_short			ip_total_length,	// Total length
					ip_id;				// Unique identifier

	u_char			ip_frag_offset :5,	// Fragment offset field

					ip_more_fragment :1,
					ip_dont_fragment :1,
					ip_reserved_zero :1,

					ip_frag_offset1,	// Fragment offset

					ip_ttl,				// Time to live
					ip_protocol;		// Protocol(TCP,UDP etc)
	u_short			ip_checksum;		// IP checksum
	u_int			ip_srcaddr,			// Source address
					ip_destaddr;		// Source address
};

struct UDPHeader
{
	u_short			source_port,		// Source port no.
					dest_port,			// Destination port no.
					udp_length,			// UDP packet length
					udp_checksum;		// UDP checksum (optional)
};

struct TCPHeader
{
	u_short			source_port,		// source port
					dest_port;			// destination port
	u_int			sequence,			// sequence number - 32 bits
					acknowledge;		// acknowledgment number - 32 bits

	u_char			ns :1,				// Nonce Sum Flag Added in RFC 3540.
					reserved_part1 :3,	// According to RFC
					data_offset :4,		// The number of 32-bit words in the TCP header.
										// This indicates where the data begins.
										// The length of the TCP header is always a multiple of 32 bits.

					fin :1,				// Finish Flag
					syn :1,				// Synchronize Flag
					rst :1,				// Reset Flag
					psh :1,				// Push Flag
					ack :1,				// Acknowledgment Flag
					urg :1,				// Urgent Flag

					ecn :1,				// ECN-Echo Flag
					cwr :1;				// Congestion Window Reduced Flag

	u_short			window,				// Window
					checksum,			// Checksum
					urgent_pointer;		// Urgent pointer
};

struct ICMPHeader
{
	u_char			type,				// ICMP Error type
					code;				// Type sub code
	u_short			checksum,
					id,
					seq;
};

}		//	owl
#endif	//	__NETWORK_HEADERS__