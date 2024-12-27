
#pragma once

#include "WebServ.hpp"

/*
Connection represents an individual client connection.
*/
class Connection
{
private:
    int _clientfd;

public:
    Connection(int clientfd);
    int get_clientfd(void);
    std::string receive();
    void close();

};