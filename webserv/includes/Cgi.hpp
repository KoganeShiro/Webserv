#pragma once

#include "WebServ.hpp"

class CGI {
private:
    std::string _name;
    std::string _compiler_path;
    std::string _extension;
    int _time_out;

public:
    CGI(std::string name, std::string compiler_path, std::string extension, int time_out);
    std::string get_name(void);
    std::string get_path(void);
    std::string get_extension(void);
    int get_time_out(void);
};

typedef struct S_Route_config
{
    std::vector<std::string> accepted_methods;
    int redirection_nb;
    std::string redirection_path;
    std::string root_dir;
    bool    dir_listing;
    bool    use_cgi;
    std::string default_file; //index.html
    S_Route_config()
        : redirection_nb(0), redirection_path(""), root_dir(""), 
        dir_listing(false), use_cgi(false), default_file(""){}
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
    std::string directory_page;

    size_t client_body_size_limit; //Sets the maximum allowed size for client request bodies

    // Routes configuration
    // Route_config *route_config;
    std::map<std::string, Route_config> routes;

    // CGI configuration
    std::vector<CGI> tab_cgi;
    // std::string cgi_path;

    // Constructor
    S_Config_data()
        : host(""), port(0), server_name(""), is_default_server(false),
          error_pages(""), directory_page(""), client_body_size_limit(0) {}

} Config_data;

/*
IN CGI.CPP
*/
std::vector<Config_data> parse_config(const char *filename);