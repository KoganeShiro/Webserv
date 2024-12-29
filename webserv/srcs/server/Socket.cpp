
#include "WebServ.hpp"

/*
void    listen_on_socket()
{
    //fill buffer until end of header (\r\n\r\n)
    //put this buffer in request_parser() param
    //on hold
    request_parser();
    //if request_parser no ERROR
    // if require body, resume and fill the buffer
    worker_response();
}

*/

//ajout Damien
void Socket::add_to_epoll(int epoll_fd) {
    epoll_event event;
    event.data.fd = this->_sockfd; // Associer le fd du socket
    event.events = EPOLLIN;       // Événement pour écouter les connexions
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_sockfd, &event) < 0) {
        throw std::runtime_error("Failed to add socket to epoll");
    }
}

Socket::Socket(int port)
{
    this->_sockfd = this->_create_socket(); // Create a new socket
    this->_bind_socket(port); // Bind the socket to the specified port
    this->_listen_for_connections(); // Start listening for incoming connections
    this->configure_epoll();//check the events happening in the socket
}

//ajout Damien
Socket::~Socket() {
    try {
        this->close_socket();
    } catch (const std::exception& e) {
        std::cerr << "Error during socket destruction: " << e.what() << std::endl;
    }
}

// Helper method to create a new socket
int Socket::_create_socket(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    return (fd);
}

// Helper method to bind the socket to an address and port
void Socket::_bind_socket(int port)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    serverAddr.sin_port = htons(port); // Convert port number to network byte order

    int opt = 1;
	if (setsockopt(_connection.get_clientfd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        throw std::runtime_error("Failed to set socket"); //For not having "Adress already in use"
	}

    if (bind(this->_sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}

// Helper method to start listening for incoming connections
void Socket::_listen_for_connections(void)
{
    if (listen(this->_sockfd, SOMAXCONN) < 0) { // Listen with maximum backlog
        throw std::runtime_error("Failed to listen on socket");
    }
}

//Wait for event to appear in the socket
void    Socket::configure_epoll(void)
{
    _epollFd = epoll_create(1);

    if (_epollFd < 0) {
        throw std::runtime_error("Epoll create failed");
    }

    epoll_event event;
    int fdSocket = _connection.get_clientfd();
    
    event.data.fd = fdSocket;
    event.events = EPOLLIN;
    epoll_ctl(_epollFd, EPOLL_CTL_ADD, fdSocket, &event);

}

void Socket::accept_connection(void)
{
    int clientfd = accept(this->_sockfd, NULL, NULL); // Accept a new connection
    if (clientfd < 0) {
        throw std::runtime_error("Failed to accept connection");
    }
    this->_connection = Connection(clientfd); // Return a Connection object for the client
}
