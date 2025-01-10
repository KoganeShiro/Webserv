
#include "Server.hpp"

Server::Server(const Config_data config): _data(config), _socket(config.port)
{
    // std::cout << GREEN
	// 	"Server constructor is called"
	// RESET << std::endl;
}

Server::~Server(){
    std::cout << "DESTROY SEVER\n";
}

void Server::add_to_epoll(int epoll_fd)
{
    epoll_event event;
    event.data.fd = get_socket_fd(); // Associer le fd du socket
    if (event.data.fd < 0) {
        std::cerr << "Invalid socket file descriptor: " << event.data.fd << std::endl;
        throw std::runtime_error("Invalid socket file descriptor");
    }
    event.events = EPOLLIN;
    std::cout << "event.data.fd = " << event.data.fd << std::endl;
    std::cout << "epoll_fd = " << epoll_fd << std::endl;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) < 0) {
        std::cerr << "Failed to add socket to epoll: " << strerror(errno) << std::endl;
        throw std::runtime_error("Failed to add socket to epoll");
    }
}

int  Server::get_socket_fd(void){
    return (_socket.get_sockfd());
}

Config_data Server::get_data(void) const{
    return (_data);
}

// std::vector <Connection> connection_tab;
Connection*    Server::add_connection(){
    int client_fd = accept(get_socket_fd(), NULL, NULL);
    if (client_fd == -1) {
        //Close le server ?
        std::cerr << "Erreur at the connexion." << std::endl;
        return (NULL);
    }

    Connection new_connection(client_fd);
        _connection_tab.push_back(new_connection);
    std::cout << "Nouvelle connexion ajoutée : fd = " << client_fd << std::endl;
    return (new Connection(new_connection));
}

// Request *init_request(Connection user_connect)
// {
//     // Request request;
//     // Request *parsed_request;
//     // std::string request_buffer; //add all the data red
    
//     //init new Request() = *this;
//     return (NULL);
// }

Connection* Server::get_connection_by_fd(int client_fd) {
    for (size_t i = 0; i < _connection_tab.size(); ++i) {
        if (_connection_tab[i].get_clientfd() == client_fd) {
            return (&_connection_tab[i]);  
        }
    }
    return (NULL);
}