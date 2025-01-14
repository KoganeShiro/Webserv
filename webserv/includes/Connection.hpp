
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
    bool    _request_is_done;
    Request request;

public:
    Connection(int clientfd);
    bool    is_request_done(void);
    int get_clientfd(void);
    Request*    get_request(void);
    void    set_request_is_done(bool is_done);

    void close();

};