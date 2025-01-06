
#include "Server.hpp"

Server::Server(const Config_data config)
{
    // std::cout << GREEN
	// 	"Server constructor is called"
	// RESET << std::endl;
    this->_data = config;
    this->_socket = Socket(_data.port);
}

//ajout Damien
void Server::add_to_epoll(int epoll_fd)
{
    this->_socket.add_to_epoll(epoll_fd);
}
/*
int  Server::get_socket_fd(){
    return (_socket.get_sockfd());
}
*/

Config_data Server::get_data() const{
    return (_data);
}


Request *init_request(Connection user_connect)
{
    (void) user_connect;
    // Request request;
    // Request *parsed_request;
    // std::string request_buffer; //add all the data red
    
    //init new Request() = *this;
    return (NULL);
}
