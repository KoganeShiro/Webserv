#pragma once

// #include "WebServ.hpp"
#include <string>


class CGI
{
    private :
        std::string _name; //Stores the CGI's name
        std::string _compiler_path; //path of the cgi
        std::string _extension; //py || php || js || ...
        int _time_out; //time to wait in second
    
    public :
        CGI(std::string name, std::string compiler_path, std::string extension, int time_out);
        std::string get_name(void);
        std::string get_path(void);
        std::string get_extension(void);
        int get_time_out(void);
} ;

