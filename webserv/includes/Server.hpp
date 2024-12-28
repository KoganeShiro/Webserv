
# pragma once

#include "WebServ.hpp"

class Server
{
private:
    Config_data _data;
    Socket _socket;
    Worker _worker;
    void _handle_connection(Connection user_connect);

public:
    Server(const std::string config);
    void run();
    int get_port();
    void add_to_epoll(int epoll_fd);//ajout Damien

    //other getter ??
};
