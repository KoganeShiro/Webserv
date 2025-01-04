#pragma once

// #include "WebServ.hpp"
#include "Cgi.hpp"
#include <map>
#include <vector>
#include <string>

CGI cgi;

typedef struct S_Route_config
{
    std::vector<std::string> accepted_methods;
    std::map<int, std::string> redirection; //Stores the URL for HTTP redirection
    int redirection_nb;
    std::string redirection_path;
    std::string root_dir;
    bool    dir_listing;
    bool    use_cgi;
    std::string default_file; //index.html
    std::string upload_dir; //dir where we upload file
} Route_config;

typedef struct S_Config_data
{
    // Server configuration
    std::string host; //ip address or hostname
    int port;
    std::string server_name; // server names (domains)
    bool is_default_server; //host:port combination OPTIONNEL ??

    // Web pages
    std::string error_pages; //template page
    // std::string method_pages;
    std::string directory_page;

    size_t client_body_size_limit; //Sets the maximum allowed size for client request bodies

    // Routes configuration
    // Route_config *route_config;
    std::map<std::string, Route_config> routes;

    // CGI configuration
    std::vector<CGI> tab_cgi;
    // std::string cgi_path;

} Config_data;