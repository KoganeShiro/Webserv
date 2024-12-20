
#pragma once

#include "WebServ.hpp"

/*
Socket represents the server-side listening socket.
*/

class Socket {
private:
    int _sockfd;
    int _create_socket();
    void _bind_socket(int port);
    void _listen_for_connections();

public:
    Socket(int port);
    ~Socket();
    Connection accept();
    int get_sockfd();
};
