#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stddef.h>

typedef	uint32_t	in_addr_t;
typedef	uint16_t	in_port_t;


#define	IPPROTO_IP		0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define 	IPPROTO_IPV6		41		/* IP6 header */
#define	IPPROTO_RAW		255		/* raw IP packet */

struct in_addr {
	in_addr_t s_addr;
};

#define	INADDR_ANY		(uint32_t)0x00000000
#define	INADDR_BROADCAST	(uint32_t)0xffffffff
#define INADDR_NONE		0xffffffff

struct sockaddr_in {
	uint8_t	        sin_len;
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct	in_addr sin_addr;
	char		sin_zero[8];
};

#define INET_ADDRSTRLEN                 16

#endif /* NETINET_IN_H */
