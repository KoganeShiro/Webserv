
#pragma once

#include "WebServ.hpp"

/*
EXEMPLE

_method /_path HTTP/1.1
Host: example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 50

_body = "
name=FirstName%20LastName&email=bsmth%40example.com
"

*/

class Request
{
private:
    std::string _method; // HTTP method (GET, POST, DELETE, etc.)
    std::string _path;   // Request path (e.g., /api/resource)
    std::string _header_str; //header fill by buffer
    std::map<std::string, std::string> _headers; // Request headers
    std::string _body;   // Request body (for POST/PUT requests)

public:
    Request();
    ~Request();

    get_method() const;
    get_path() const;
    get_header() const;
    std::string get_header_element(const std::string& key) const;
    get_body() const;

    void set_method(const std::string& method);
    void set_path(const std::string& path);
    void add_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);

};
