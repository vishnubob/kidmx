#ifndef KINET_H
#define KINET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdexcept>
#include "rpidmx.h"

#define KINET_UDP_PORT          6038
#define KINET_VERSION           0x0001

// KiNET magic numbers
#define KINET_MAGIC             0x4adc0104
#define KINET_MORE_MAGIC        0xdeadbeef
#define KINET_DEEP_MAGIC        0xc001d00d
#define KINET_MAGIC_HASH        0x69000420

#define KTYPE_DMXOUT            0x0101
#define KTYPE_SET               0x0103
#define KSET_ACKS               1

#define KTYPE_KDM_SER9OUT 0xfb02
#define KTYPE_KDM_TICKER 0xfb03
#define KTYPE_KDM_DMXSTROBE 0xfb04

#define KINET_HEADER_SIZE 21
#define BUFFER_SIZE 1000

class KinetSocket 
{
public:
    KinetSocket(char *ipaddr);
    void receive(channel_vec_t &channels);
    int _sockfd;
};

#endif /* KINET_H */
