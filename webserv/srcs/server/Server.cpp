
#include "Server.hpp"

Server::Server(const Config_data config): _data(config), _socket(config.port, config.host, config.server_name) {}

Server::~Server()
{
    std::cout << "DESTROY SEVER\n";
}

int  Server::get_socket_fd(void)
{
    return (_socket.get_sockfd());
}

Config_data Server::get_data(void) const
{
    return (_data);
}

// std::vector <Connection> connection_tab;
Connection*    Server::add_connection()
{
    int client_fd = accept(get_socket_fd(), NULL, NULL);

    if (client_fd == -1) {
        //Close le server ?
        std::cerr << "Error at Server-Socket : " << get_socket_fd() << std::endl;
        return (NULL);
    }
    
    Connection new_connection(client_fd);
    _connection_tab.push_back(new_connection);
    std::cout << BLUE "Added Client-Socket " RESET << client_fd << BLUE " received from Server-Socket " RESET << get_socket_fd() <<  std::endl;
    return (new Connection(new_connection));
}

void    Server::remove_connection(int client_fd)
{
    for (size_t i = 0; i < _connection_tab.size(); ++i) {
        if (_connection_tab[i].get_clientfd() == client_fd) {
            _connection_tab.erase(_connection_tab.begin() + i);
            break;
        }
    }    
}

Connection* Server::get_connection_by_fd(int client_fd)
{
    for (size_t i = 0; i < _connection_tab.size(); ++i) {
        if (_connection_tab[i].get_clientfd() == client_fd) {
            return (&_connection_tab[i]);  
        }
    }
    return (NULL);
}
