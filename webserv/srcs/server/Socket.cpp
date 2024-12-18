
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

Socket::Socket(int port)
{
    this->sockfd = createSocket(); // Create a new socket
    bindSocket(port); // Bind the socket to the specified port
    listenForConnections(); // Start listening for incoming connections
}

// Helper method to create a new socket
int Socket::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    return fd;
}

// Helper method to bind the socket to an address and port
void Socket::bindSocket(int port)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    serverAddr.sin_port = htons(port); // Convert port number to network byte order

    if (bind(this->sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}

// Helper method to start listening for incoming connections
void Socket::listenForConnections()
{
    if (listen(this->sockfd, SOMAXCONN) < 0) { // Listen with maximum backlog
        throw std::runtime_error("Failed to listen on socket");
    }
}


Connection Socket::accept()
{
    int clientfd = ::accept(this->sockfd, NULL, NULL); // Accept a new connection
    if (clientfd < 0) {
        throw std::runtime_error("Failed to accept connection");
    }
    return Connection(clientfd); // Return a Connection object for the client
}
