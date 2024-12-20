
#pragma once

# define RED "\033[1;31m"
# define ORANGE "\033[38;2;255;145;5;3m"
# define BGREEN "\033[0m\033[38;2;0;170;0;1m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"
# define RESET "\033[0m"

#include <map>
#include <vector>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <iostream>
#include <fcntl.h>
#include <sys/wait.h>
#include <csignal>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

#include "Server.hpp"
#include "Socket.hpp"
#include "DeleteMethod.hpp"
#include "GetMethod.hpp"
#include "PostMethod.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Worker.hpp"
#include "Connection.hpp"


//maybe do classes ?
//main include file

struct Config_data
{
    int nb_of_server;
        //make an array of data ???
    std::string server_name;
    int listen_port;
        //can have multiple port, array of int ??
    int timeout;
    std::string root_path;
    std::string *methods; //array of GET POST DELETE
    std::string cgi_path;

};


std::string replace_string(std::string res, std::string to_replace, std::string replace_with);
    //could use the same logic as sed

const std::string &gen_htmlbody();
