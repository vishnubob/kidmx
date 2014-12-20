#include "kinet.h"

KinetSocket::KinetSocket(char *ipaddr)
{
    int errnum;
    char *errmsg;
    struct in_addr addr;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));

    errnum = inet_aton(ipaddr, &servaddr.sin_addr);
    if (errnum == 0)
    {
        throw std::runtime_error("Invalid server address");
    }

    _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sockfd < 0)
    {
        errmsg = strerror(errnum);
        throw std::runtime_error(errmsg);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(KINET_UDP_PORT);
    errnum = bind(_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (errnum < 0)
    {
        errmsg = strerror(errnum);
        throw std::runtime_error(errmsg);
    }
}

void KinetSocket::receive(channel_vec_t &channels)
{
    struct sockaddr_in cliaddr;
    uint8_t buf[BUFFER_SIZE];
    uint32_t sz;
    socklen_t len = sizeof(cliaddr);

    sz = recvfrom(_sockfd, buf, 1000, 0, (struct sockaddr *)&cliaddr, &len);

    for(int idx = 0; idx < DMX_CHANNEL_COUNT; idx += 1)
    {
        channels[idx] = buf[KINET_HEADER_SIZE + idx];
    }
}
