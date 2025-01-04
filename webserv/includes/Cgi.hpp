#pragma once

// #include "Config.hpp"
// #include "WebServ.hpp"
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

#define EXPLAINATION "A VALID config file should follow this format :\n\t\
-A server initialisation should start whith 'server {' and nothing else on the line\n\t\
-'host' should be follow by a IP adress\n\t\
-'listen' should be follow by a space and a number corresponding to a port and nothing else\n\t\
-'server_name'(facultative) should be follow by a space and the name of the server in ONE word\n\t\
-'client_max_body_size' should be follow by a space and a value terminate by K, M or G all in one word\n\t\
-'default_file' should be follow by an url\n\t\
-'directory_page' should be follow by a space and the path of an html file\n\t\
-'location' should be follow by a path and a '{'\n\t\t\
-In 'location', 'method' should be follow by at least one of GET PUSH or DELETE\n\t\t\
-In 'location', 'return' follow by 301,302,303,307 or 308 should be follow by an url\n\t\t\
-In 'location', 'root' follow by the path of a directory\n\t\t\
-In 'location', 'autoindex' follow by ON or OFF(default)\n\t\t\
-In 'location', 'index' follow by a file path\n\t\t\
-In 'location', 'upload_store' follow by a directory path\n\t\
-In 'location', 'use_cgi' follow by YES or NO(default)\n\t\
-Another '}' should be use alone on a newline to close a 'location'\n\t\
-And another '}' should be use alone on a newline to close a 'server'\n\n\
-Cgi should be declare like this :\n\
cgi {\n\
\tlanguage name {\n\
\t\tcgi_path path;\n\
\t\tcgi_extension .extension ;\n\
\t\tcgi_timeout time in second > 0;\n\
-And another '}' should be use alone on a newline to close a 'cgi'\n\
\t}\n\
}\n\n\n"

// Config_data data;

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

    // Constructor
    S_Config_data()
        : host(""), port(0), server_name(""), is_default_server(false),
          error_pages(""), directory_page(""), client_body_size_limit(0) {}

} Config_data;

// Déclaration des fonctions de parsing
std::vector<CGI> parse_cgis(std::ifstream& file, std::string line);
std::vector<Config_data> parse_config(const char *filename);  // Déclaration dans l'en-tête principal