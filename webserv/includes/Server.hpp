
# pragma once

#include "WebServ.hpp"

class Server
{
private:
    Config_data _data;
    Socket _socket;
    //Worker _worker;
    std::vector <Connection> connection_tab;//ajout Damien

public:
    Server(const std::string config);
    void run();
    int get_port() const;
    int  get_socket_fd() const;

    void    add_connection(void);//ajout Damien
    void add_to_epoll(int epoll_fd);//ajout Damien

    //other getter ??
};
