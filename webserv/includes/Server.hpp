
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
};

class ServerManager
{
public:
    static std::vector<Server*> servers;

    static void cleanup() {
        for (size_t i = 0; i < servers.size(); ++i) {

          //  servers[i]->get_socket_fd().close();
            delete servers[i];
            
            std::cout << "server " << i << "deleted\n"; 
        }
        std::cout << "server cleaned\n"; 

        servers.clear();
    }
};