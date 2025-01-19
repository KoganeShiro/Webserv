
#pragma once

#include "WebServ.hpp"
#include "Connection.hpp"

/*
Socket represents the server-side listening socket.
*/

class Socket
{
private:
    std::vector<Connection>  _connection;
    epoll_event events_tab[42];
    int     _epollFd;
    int     _sockfd;
    int     _create_socket(void);
    void    _bind_socket(int port, std::string host, std::string server_name);
    //void    configure_epoll(void);
    void    _listen_for_connections(void);

public:
    Socket(int port, std::string host, std::string server_name);
    ~Socket();
    void accept_connection();
   int get_sockfd();
    void add_to_epoll(int epoll_fd);

};
