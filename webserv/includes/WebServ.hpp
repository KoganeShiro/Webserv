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

template <typename T>
std::string to_string(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#define MAX_EVENTS 10
#define MAX_FILE_SIZE 1000000
#define MAX_SERVERS 10
#define STD_BODY_SIZE 5000

//DEFINE for Server
#define BAD_CONFIG RED "Your file '.config' doesn't configure a server as requested\n" RESET

//DEFINE for Cgi
#define EXPLAINATION "A VALID config file should follow this format :\n\t\
-Each line with a declaration should end with a ';'\
-A server initialisation should start whith 'server {' and nothing else on the line\n\t\
-'host' should be follow by a IP adress\n\t\
-'listen' should be follow by a space and a number corresponding to a port and nothing else\n\t\
-'server_name'(facultative) should be follow by a space and the name of the server in ONE word\n\t\
-'client_max_body_size' should be follow by a space and a value terminate by K, M or G all in one word,\
if none is given, the default size is fixed at 5000\n\t\
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
}\n\n\
If any of those rule is not apply, it might conduct to a bad configured server and continue the program\
\n\n\n"

//DEFINE for Request
#define MAX_HEADER_SIZE 8000
#define AGAIN 0
#define BAD_HEADER 1
#define GOOD 2
#define MULTIPART_FORM_DATA 3

//DEFINE for run.cpp
#define READ_PROBLEM -1

/*
** utils/request_parsing.cpp
*/
std::string extract_boundary(const std::string& content_type);
std::string clean_request_body(const std::string& buffer);