
#pragma once

#include "WebServ.hpp"

/*
array of status_code and its corresponding
status_message
*/

class Response {
private:
    int _status_code;
    std::string _status_message; //
    std::map<std::string, std::string> _headers;
    std::string _body;

public:
    Response();
    Response(int statusCode, const std::string& statusMessage);
    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    std::string http_response() const; //call generate_error_page

    std::string get_status_message(int _status_code);
    std::string read_html_file(std::string file_path);
};
