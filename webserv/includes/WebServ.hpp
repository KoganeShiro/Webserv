
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
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <sstream>

//#include "Cgi.hpp"
//#include "Config_data.hpp"
//#include "Request.hpp"
//#include "Response.hpp"
// #include "Server.hpp"
// #include "Socket.hpp"
//#include "DeleteMethod.hpp"
//#include "IHttpMethod.hpp"
//#include "GetMethod.hpp"
//#include "PostMethod.hpp"

//#include "Worker.hpp"
//#include "Connection.hpp"

// #include "Server.hpp"
// #include "Socket.hpp"
// #include "DeleteMethod.hpp"
// #include "GetMethod.hpp"
// #include "PostMethod.hpp"
#include "Request.hpp"
// #include "Response.hpp"
// #include "Worker.hpp"
#include "Connection.hpp"
#include "Cgi.hpp"

//main include file
#include <sstream>
#include <string>


template <typename T>
std::string to_string(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

//#define BUFFER_SIZE 4096
#define MAX_EVENTS 10
#define MAX_FILE_SIZE 1000000
#define MAX_SERVERS 10
//ServerConfig servers[MAX_SERVERS];
//int server_count = 0; // Nombre de serveurs actifs




/*
Config_data config_parser(const std::string config);

Request *init_request(Connection user_connect);
*/
// std::string replace_string(std::string res, std::string to_replace, std::string replace_with);
//     //could use the same logic as sed

// const std::string &gen_htmlbody();
