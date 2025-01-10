
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
        std::vector <Connection> _connection_tab;

    public:
        // Server()
        ~Server();
        Server(const Config_data config);
        Config_data get_data() const;
        int  get_socket_fd();

        Connection* add_connection(void);
        void add_to_epoll(int epoll_fd);
        Connection* get_connection_by_fd(int client_fd);

    //other getter ??
};
