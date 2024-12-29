
#include "WebServ.hpp"

Server::Server(std::string const config)
{
    std::cout << GREEN
		"Server constructor is called"
	RESET << std::endl;
    this->_data = config_parser(config);
    this->_socket = Socket(_data.port);
}

//ajout Damien
void Server::add_to_epoll(int epoll_fd)
{
    this->_socket.add_to_epoll(epoll_fd);

int  Server::get_socket_fd()
{
    return (this->_socket.get_sockfd());
}

Request *init_request(Connection user_connect)
{
    Request request;
    Request *parsed_request;
    std::string request_buffer; //add all the data red
    
    //init new Request() = *this;
    return (parsed_request);
}

//Dans le main faire un run d'epoll
// void Server::run()
// {
//     while (true) { // Main loop to keep the server running
//         try {
//             this->_socket = Socket(this->get_port());

// // it seems we need something here to "select" or "poll"...
		
//             Connection user_connect(this->_socket.get_sockfd()); // Accept a new client connection
//             _handle_connection(user_connect); // Handle the accepted connection
//         } 
//         catch (const std::exception& e) {
//             std::cerr << "Error accepting connection: " << e.what() << std::endl;
//             continue; // Continue to the next iteration of the loop
//         }
//     }
// }
