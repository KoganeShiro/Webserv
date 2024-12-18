
#pragma once

#include "WebServ.hpp"

/*
Socket represents the server-side listening socket.
*/

class Socket {
private:
    int sockfd;
    int createSocket();
    void bindSocket(int port);
    void listenForConnections();

public:
    Socket(int port);
    Connection accept();
};
