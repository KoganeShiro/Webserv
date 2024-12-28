
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

#include "Server.hpp"
#include "Socket.hpp"
#include "DeleteMethod.hpp"
#include "GetMethod.hpp"
#include "PostMethod.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Worker.hpp"
#include "Connection.hpp"

//main include file

// ---------------Damien---------------------
#define BUFFER_SIZE 4096
#define MAX_EVENTS 10

#define MAX_SERVERS 10
ServerConfig servers[MAX_SERVERS];
int server_count = 0; // Nombre de serveurs actifs

// struct ServerConfig 
// {
//     int port;              // Port
//     char root_dir[256];    
//     int socket_fd;         // Socket fd
// };
// ---------------END Damien-----------------

typedef struct S_Route_config
{
    std::vector<std::string> accepted_methods;
    std::string redirection; //Stores the URL for HTTP redirection
    std::string root_dir;
    bool dir_listing;
    std::string default_file; //index.html
    std::string cgi_extension; //.php ?
    std::string upload_dir; //dir where we upload file

} Route_config;

typedef struct S_Config_data
{
    // Server configuration
    std::string host; //ip address or hostname
    int port;
    std::vector<std::string> server_names; // server names (domains)
    bool is_default_server; //host:port combination OPTIONNEL ??

    // Web pages
    std::string error_pages; //template page
    std::string method_pages;

    size_t client_body_size_limit; //Sets the maximum allowed size for client request bodies

    // Routes configuration
    Route_config *route_config;
    std::map<std::string, Route_config> routes;

    // CGI configuration
    std::string cgi_path;

} Config_data;



// std::string replace_string(std::string res, std::string to_replace, std::string replace_with);
//     //could use the same logic as sed

// const std::string &gen_htmlbody();
