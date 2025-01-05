
# pragma once

#include "WebServ.hpp"
#include "Cgi.hpp"
// #include "Connection.hpp"
#include "Socket.hpp"

class Server
{
private:
    Config_data _data;
    Socket _socket;
    //Worker _worker;
    std::vector <Connection> connection_tab;//ajout Damien

public:
    Server(const Config_data config);
    Config_data get_data() const;
    int  get_socket_fd();

    void    add_connection(void);//ajout Damien
    void add_to_epoll(int epoll_fd);//ajout Damien

    //other getter ??
};
