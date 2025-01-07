
#include "../../includes/Connection.hpp"

// Constructor that initializes the connection with the client file descriptor
Connection::Connection(int clientfd) : _clientfd(clientfd)
{}
/*
// Method to receive data from the client
std::string Connection::receive()
{
    //A faire ?
    
}
*/

// Method to close the connection
void Connection::close()
{
    ::close(this->_clientfd); // Close the socket connection
    this->_clientfd = -1; // Set file descriptor to an invalid state
}

int Connection::get_clientfd(void)
{
    return (this->_clientfd);
}

