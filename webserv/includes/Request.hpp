
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

class Request
{
private:
    std::string _request_buffer;
    std::string _request; //final request
    std::map<std::string, std::string> _headers; // Request headers
    std::string _method; // HTTP method (GET, POST, DELETE, etc.)
    std::string _path;   // Request path (e.g., /api/resource)
    std::string _http_version;
    size_t _content_length;
    std::string _body;   // Request body (for POST/PUT requests)
    bool _good_request;
    int _is_ready;
    size_t _pos;
    bool _finish_header;
    Request request_parser(Request &request, std::string& buffer, size_t MAX_BODY_LENGTH);

public:
    Request();
    Request(Request const &other);
    Request &operator=(Request const &other);
    ~Request();

    std::string get_method() const;
    std::string get_path() const;
    //std::string get_all_header() const;
    std::string get_header_element(const std::string& key) const;
    std::map<std::string, std::string> get_header() const;
    std::string get_body() const;
    bool get_good_request() const;
    int get_is_ready() const;
    std::string get_request_buffer() const ;
    std::string get_http_version() const ;
    size_t get_content_length() const ;
    size_t get_pos() const;

    void set_pos(size_t pos);
    void set_method(const std::string& method);
    void set_path(const std::string& path);
    void add_header(const std::string& key, const std::string& value);
    void set_body(const std::string& body);
    void set_good_request(const bool all_good);
    void set_is_ready(const int ready);
    void set_request_buffer(const std::string request_buffer);
    void set_http_version(const std::string http_version);
    void set_content_length(size_t length);
    void set_to_null();

    /* IN REQUEST_PARSER */
    void set_finish_header(bool finish);
    bool get_finish_header();
    int add_to_request(std::string to_add, size_t MAX_BODY_LENGTH);

    Request *parsed_request();
};
