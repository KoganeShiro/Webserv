
#pragma once

#include "WebServ.hpp"
#include "Cgi.hpp"

/*
array of status_code and its corresponding
status_message
*/

class Response
{
private:
    int _statusCode;
    std::string _statusMessage;
    std::map<std::string, std::string> _headers;
    std::string _body;
    bool _header_and_body_in_one;
    Config_data _config;
    std::string generate_error_page(int status_code, std::string status_message);
    std::string generate_directory_listing(std::string directory);

public:
    Response();
    Response(const std::string& header_and_body);
    Response(int statusCode, const std::string& statusMessage, Config_data c);
    Response(int statusCode, const std::string& statusMessage, std::string directory, bool isDirectory, Config_data c);
    Response(int statusCode, const std::string& statusMessage, const std::string& body, Config_data c);
    Response(int statusCode, const std::string& statusMessage, const std::string& header_and_body);
    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    std::string http_response() const; //call generate_error_page
    Response &operator=(const Response &response);

};
