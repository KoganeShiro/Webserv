
#include "WebServ.hpp"

// Constructor that initializes the connection with the client file descriptor
Connection::Connection(int clientfd) : _clientfd(clientfd)
{}

// Method to receive data from the client
std::string Connection::receive()
{
    char buffer[1024]; // Buffer to hold incoming data

    /*
    int bytesRead = read(clientfd, buffer, sizeof(buffer) - 1); // Read data from socket
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        return std::string(buffer); // Return received data as a string
    } */
    read until get the end of the request_header
    if (everything is ok) {
        return (request_header);
    }
    else {
        return ""; // Return empty string on error or no data
    }
}

// Method to close the connection
void Connection::close() {
    ::close(this->_clientfd); // Close the socket connection
    this->_clientfd = -1; // Set file descriptor to an invalid state
}
