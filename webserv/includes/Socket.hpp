
#pragma once

#include "WebServ.hpp"

/*
Socket represents the server-side listening socket.
*/

class Socket {
private:
    Connection  _connection;
    epoll_event events_tab[42];
    int     _epollFd;
    int     _sockfd;
    int     _create_socket(void);
    void    _bind_socket(int port);
    void    configure_epoll(void);
    void    _listen_for_connections(void);

public:
    Socket();
    Socket(int port);
    ~Socket();
    void accept_connection();
    int get_sockfd();
    void add_to_epoll(int epoll_fd);//ajout Damien

};
