
# pragma once

#include "WebServ.hpp"
#include "Cgi.hpp"
#include "Socket.hpp"

#define MAX_OPEN_CONNECTIONS 10

class Server
{
private:
    Config_data _data;
    Socket _socket;
    std::vector <Connection> _connection_tab;

public:
    ~Server();
    Server(const Config_data config);
    Config_data get_data() const;
    int  get_socket_fd();

    Connection* add_connection(void);
    void remove_connection(int client_fd);

    Connection* get_connection_by_fd(int client_fd);
};

class ServerManager
{
public:
    static std::vector<Server*> servers;

    static void cleanup() {
        for (size_t i = 0; i < servers.size(); ++i) {
            delete servers[i];
            std::cout << "server " << i << "deleted\n"; 
        }
        std::cout << "server cleaned\n"; 

        servers.clear();
    }
};