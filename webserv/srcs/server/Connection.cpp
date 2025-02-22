
#include "Connection.hpp"

// Constructor that initializes the connection with the client file descriptor
Connection::Connection(int clientfd) : _clientfd(clientfd)
{
    this->_request_is_done = 1;
    Request req;
    this->request = req;
}

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

bool    Connection::is_request_done(void)
{
    return (this->_request_is_done);
}

void    Connection::set_request_is_done(bool is_done)
{
    this->_request_is_done = is_done;
}

Request*    Connection::get_request(void)
{
    return (&request);
}
