#include "../includes/Cgi.hpp"

CGI::CGI(std::string name, std::string compiler_path, std::string extension, int time_out){
    this->_name = name;
    this->_compiler_path = compiler_path;
    this->_extension = extension;
    this->_time_out = time_out;
}
        
std::string CGI::get_name(void){
    return (this->_name);
}

std::string CGI::get_path(void){
    return (this->_compiler_path);
}
        
std::string CGI::get_extension(void){
    return (this->_extension);
}

int CGI::get_time_out(void){
    return (this->_time_out);
}