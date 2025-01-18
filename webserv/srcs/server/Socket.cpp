
#include "Socket.hpp"

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
void Socket::add_to_epoll(int epoll_fd)
{
    epoll_event event;
    event.data.fd = this->_sockfd; // Associate the socket file descriptor
    event.events = EPOLLIN;       // Event in order to listen to connections
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_sockfd, &event) < 0) {
        throw std::runtime_error("Failed to add socket to epoll");
    }
}

Socket::Socket(int port, std::string host, std::string server_name)
{
    this->_sockfd = this->_create_socket(); // Create a new socket
    this->_bind_socket(port, host, server_name); // Bind the socket to the specified port
    this->_listen_for_connections(); // Start listening for incoming connections

    // this->configure_epoll();//check the events happening in the socket
}

//ajout Damien
Socket::~Socket() {
    std::cout << "Closing socket fd = " << _sockfd << std::endl;
    try {
        close(_sockfd);
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
void Socket::_bind_socket(int port, std::string host, std::string server_name)
{
    (void)server_name;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(port); // Convert port number to network byte order

    std::cout << "Trying to bind to " << host << ":" << port << std::endl;

    // Resolve hostname to IP address
    struct hostent* he = gethostbyname(host.c_str());
    if (he == NULL) {
        close(_sockfd);
        throw std::runtime_error("Failed to resolve hostname: " + host);
    }

    // Copy the resolved IP address into server_addr
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    // Set SO_REUSEADDR option
    int opt = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(_sockfd);
        throw std::runtime_error("Failed to set socket options: " + std::string(strerror(errno)));
    }

    // Bind the socket
    if (bind(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        int err = errno; // Capture errno before any other function call
        close(_sockfd);
        throw std::runtime_error("Failed to bind socket: " + std::string(strerror(err)));
    }
    
    std::cout << "Socket bound successfully on " << host << ":" << port << std::endl;
}


// Helper method to start listening for incoming connections
void Socket::_listen_for_connections(void)
{
    if (listen(this->_sockfd, SOMAXCONN) < 0) { // Listen with maximum backlog
        throw std::runtime_error("Failed to listen on socket");
    }
}

// //Wait for event to appear in the socket
// void    Socket::configure_epoll(void)
// {
//     _epollFd = epoll_create(1);
//     std::cout << _epollFd << "|||||||||\n"; 
//     if (_epollFd < 0) {
//         throw std::runtime_error("Epoll create failed");
//     }

//     epoll_event event;    
//     event.data.fd = _sockfd;
//     event.events = EPOLLIN;
//     epoll_ctl(_epollFd, EPOLL_CTL_ADD, _sockfd, &event);

// }

void Socket::accept_connection(void)
{
    int clientfd = accept(this->_sockfd, NULL, NULL); // Accept a new connection
    if (clientfd < 0) {
        throw std::runtime_error("Failed to accept connection");
    }
    this->_connection.push_back(Connection(clientfd)); // Return a Connection object for the client
}

int Socket::get_sockfd()
{
    return (this->_sockfd);
}
