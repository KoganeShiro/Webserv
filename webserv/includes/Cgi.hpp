#pragma once

#include "WebServ.hpp"

class CGI {
public:
    std::string _name;
    std::string _compiler_path;
    std::string _extension;
    int _time_out;


    CGI(std::string name, std::string compiler_path, std::string extension, int time_out);
    std::string get_name(void);
    std::string get_path(void);
    std::string get_extension(void);
    int get_time_out(void);
    CGI(){
        _name = "";
        _compiler_path = "";
        _extension = "";
        _time_out = 0;
    };
    // Copy constructor
    CGI(const CGI& other)
        : _name(other._name), _compiler_path(other._compiler_path), _extension(other._extension), _time_out(other._time_out) {}

    // Copy assignment operator
    CGI& operator=(const CGI& other) {
        if (this == &other)
            return *this;
        _name = other._name;
        _compiler_path = other._compiler_path;
        _extension = other._extension;
        _time_out = other._time_out;
        return *this;
    }

    // Destructor (default is fine)
    ~CGI() {}
    

};

class Route_config
{
public:
    std::vector<std::string> accepted_methods;
    int redirection_nb;
    std::string redirection_path;
    std::string root_dir;
    bool    dir_listing;
    bool    use_cgi;
    std::string default_file; //index.html
  //  std::string upload_dir; //dir where we upload file
 

    // Default constructor
    Route_config() : redirection_nb(0), dir_listing(false), use_cgi(false) {}

    // Copy constructor
    Route_config(const Route_config& other)
        : accepted_methods(other.accepted_methods), redirection_nb(other.redirection_nb),
          redirection_path(other.redirection_path), root_dir(other.root_dir),
          dir_listing(other.dir_listing), use_cgi(other.use_cgi), default_file(other.default_file) {}

    // Copy assignment operator
    Route_config& operator=(const Route_config& other) {
        if (this == &other)
            return *this;
        accepted_methods = other.accepted_methods;
        redirection_nb = other.redirection_nb;
        redirection_path = other.redirection_path;
        root_dir = other.root_dir;
        dir_listing = other.dir_listing;
        use_cgi = other.use_cgi;
        default_file = other.default_file;
        return *this;
    }

    // Destructor
    ~Route_config() {}


} ;

class Config_data
{
public:
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

    // Constructor
    

    Config_data()
        : host(""), port(0), server_name(""), is_default_server(false),
          error_pages(""), directory_page(""), client_body_size_limit(0) {}

    // Copy constructor
    Config_data(const Config_data& other)
        : host(other.host), port(other.port), server_name(other.server_name),
          is_default_server(other.is_default_server), error_pages(other.error_pages),
          directory_page(other.directory_page), client_body_size_limit(other.client_body_size_limit),
          routes(other.routes), tab_cgi(other.tab_cgi) {}

    // Copy assignment operator
    Config_data& operator=(const Config_data& other) {
        if (this == &other)
            return *this;
        host = other.host;
        port = other.port;
        server_name = other.server_name;
        is_default_server = other.is_default_server;
        error_pages = other.error_pages;
        directory_page = other.directory_page;
        client_body_size_limit = other.client_body_size_limit;
        routes = other.routes;
        tab_cgi = other.tab_cgi;
        return *this;
    }

    // Destructor
    ~Config_data() {}

} ;

// Déclaration des fonctions de parsing
// std::vector<CGI> parse_cgis(std::ifstream& file, std::string line);
std::vector<Config_data> parse_config(const char *filename);  // Déclaration dans l'en-tête principal