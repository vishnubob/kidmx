#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>

#include "kinet.h"
#include "rpidmx.h"

// set epoll timeout to 1ms
#define EPOLL_TIMEOUT 1

std::vector<KinetSocket*> _sockets;
pin_vec_t _pins;

void run()
{
    int ready = 0;
    channel_vec_t channels(DMX_CHANNEL_COUNT, 0);
    RPIDMX _rpidmx(_pins);
    int epfd = epoll_create(_sockets.size());

    for (uint32_t idx = 0; idx < _sockets.size(); idx += 1)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.u32 = idx;
        epoll_ctl(epfd, EPOLL_CTL_ADD, _sockets[idx]->_sockfd, &ev);
    }

    struct epoll_event *evlist = new struct epoll_event[_sockets.size()];

    while(1)
    {
        ready = epoll_wait(epfd, evlist, _sockets.size(), EPOLL_TIMEOUT);
        if (ready)
        {
            for (int idx = 0; idx < _sockets.size(); ++idx)
            {
                if (evlist[idx].events & EPOLLIN) 
                {
                    _sockets[evlist[idx].data.u32]->receive(channels);
                    _rpidmx.update(evlist[idx].data.u32, channels);
                }
            }
        }
        _rpidmx.refresh();
    }
}

void cli(int argc, char *argv[])
{
    uint32_t pin;
    char *ip;

    for (int idx = 1; idx < argc; idx += 2)
    {
        pin = atoi(argv[idx]);
        ip = argv[idx + 1];
        KinetSocket *socket = new KinetSocket(ip);
        _sockets.push_back(socket);
        _pins.push_back(pin);
    }
}

int main(int argc, char **argv)
{
    cli(argc, argv);
    run();
    return 0;
}
