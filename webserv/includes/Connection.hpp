
#pragma once

#include "WebServ.hpp"

/*
Connection represents an individual client connection.
*/
class Connection
{
private:
    int clientfd;

public:
    Connection(int clientfd);
    std::string receive();
    void close();

};