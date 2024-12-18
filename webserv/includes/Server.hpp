
# pragma once

#include "WebServ.hpp"

class Server
{
private:
    Socket socket;
    Worker worker;
    void handle_connection();

public:
    Server();
    void run();
};
