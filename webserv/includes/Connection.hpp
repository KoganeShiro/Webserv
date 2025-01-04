
#pragma once

#include "WebServ.hpp"
#include "Request.hpp"

class Request;

/*
Connection represents an individual client connection.
*/
class Connection
{
private:
    int _clientfd; //---> sockfd
    Request *request;

public:
    Connection(int clientfd);
    int get_clientfd(void);
    std::string receive();
    void close();

};