
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

/*
checked body length

check for chunked

check if right host ?
*/

#define ENCORE 0
#define HEADER_TOO_LONG 1
#define GOOD 2

class Request
{
private:
    std::string _request_buffer;
    std::string _request; //final request
    std::map<std::string, std::string> _headers; // Request headers
    std::string _method; // HTTP method (GET, POST, DELETE, etc.)
    std::string _path;   // Request path (e.g., /api/resource)
    std::string _http_version;
    int _content_length;
    std::string _body;   // Request body (for POST/PUT requests)
    bool _good_request;
    int _is_ready;

public:
    Request();
    Request(Request const &other);
    ~Request();

    std::string get_method() const;
    std::string get_path() const;
    std::string get_all_header() const;
    std::string get_header_element(const std::string& key) const;
    std::map<std::string, std::string> get_header() const;
    std::string get_body() const;
    bool get_good_request() const;
    int get_is_ready() const;
    std::string get_request_buffer() const ;
    std::string get_http_version() const ;
    int get_content_length() const ;

    void set_method(const std::string& method);
    void set_path(const std::string& path);
    void add_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    void set_good_request(const bool all_good);
    void set_is_ready(const int ready);
    void set_request_buffer(const std::string request_buffer);
    void set_http_version(const std::string http_version);
    void set_content_length(int lenght);
    void set_to_null();

    /* IN REQUEST_PARSER */
    void add_to_request(std::string to_add);
    Request *parsed_request();
};
