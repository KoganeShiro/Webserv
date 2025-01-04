#pragma once

//#include "WebServ.hpp"

#include "Cgi.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#define MAX_SERVERS 10
// int server_count = 0; // Nombre de serveurs actifs

struct ServerConfig 
{
    int port;              // Port
    char root_dir[256];    
    int socket_fd;         // Socket fd
};
// ---------------END Damien-----------------
// ServerConfig servers[MAX_SERVERS];
typedef struct S_Route_config
{
    std::vector<std::string> accepted_methods;
    std::string redirection; //Stores the URL for HTTP redirection
    int type_redir;
    std::string root_dir;
    bool dir_listing;
    bool use_cgi;
    std::string default_file; //index.html    
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
    std::string directory_page;

    size_t client_body_size_limit; //Sets the maximum allowed size for client request bodies

    // Routes configuration
    Route_config *route_config;
    std::map<std::string, Route_config> routes;

    // CGI configuration
    std::vector<CGI> *tab_cgi;
    std::string cgi_path;

} Config_data;