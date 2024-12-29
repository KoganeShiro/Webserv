
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
    std::string _request; //final request
    std::map<std::string, std::string> _headers; // Request headers
    std::string _method; // HTTP method (GET, POST, DELETE, etc.)
    std::string _path;   // Request path (e.g., /api/resource)
    std::string _header_str; //header fill by buffer
    std::string _body;   // Request body (for POST/PUT requests)
    bool _good_request;
    bool _is_ready;

public:
    Request();
    ~Request();

    std::string get_method() const;
    std::string get_path() const;
    std::string get_header() const;
    std::string get_header_element(const std::string& key) const;
    std::string get_body() const;
    bool get_good_request() const;
    std::string get_request() const;

    void set_method(const std::string& method);
    void set_path(const std::string& path);
    void add_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    void set_good_request(const bool ok);
    void add_to_request(std::string to_add);

};
