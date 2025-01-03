
#pragma once

//#include "WebServ.hpp"
#include "Config_data.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <dirent.h>
#include <sys/types.h>


/*
array of status_code and its corresponding
status_message
*/

class Response {
private:
    int _statusCode;
    std::string _statusMessage; //
    std::map<std::string, std::string> _headers;
    std::string _body;
    Config_data _config;
    std::string generate_page(int status_code, std::string status_message);
    std::string generate_directory_listing(std::string directory);

public:
    Response();
    Response(int statusCode, const std::string& statusMessage, Config_data c);
    Response(int statusCode, const std::string& statusMessage, std::string directory);
    Response(int statusCode, const std::string& statusMessage, const std::string& body, Config_data c);
    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    std::string http_response() const; //call generate_error_page
    Response &operator=(const Response &response);

  //  std::string get_status_message(int _status_code);
   // std::string read_html_file(std::string file_path);
};
