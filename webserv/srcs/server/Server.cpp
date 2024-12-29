
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
void Server::add_to_epoll(int epoll_fd) {
    this->_socket.add_to_epoll(epoll_fd);

int  Server::get_socket_fd()
{
    return (this->_socket.get_sockfd());
}

void Server::_handle_connection(Connection user_connect)
{
    //dunno if try catch is the right struct
    this-> _worker = Worker(this->_socket); //make a setter function ?
    try {
        std::string request = user_connect.receive(); // Receive request from client
        //fill buffer until end of header (\r\n\r\n)
        //put this buffer in request_parser() param
        //on hold
        if (!request.empty() && find("\r\n\r\n")) {
            //maybe have a better class init ??
            Request parsed_request = request_parser(request); // Parse the incoming request
            this->_worker.process_request(parsed_request); // Use Worker to process the request
            //if request_parser no ERROR
            // if require body, resume and fill the buffer
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
    //if everything is done, we can close this session
    user_connect.close(); // Ensure that the connection is closed after handling
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
