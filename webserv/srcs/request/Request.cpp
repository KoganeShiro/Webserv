
#include "WebServ.hpp"

Request::Request() :_request_buffer(""),
    _request(""), _headers(), _method(""),
    _path(""), _http_version(""),
    _content_length(0), _body(""),
    _good_request(false),
    _is_ready(ENCORE)
{}

Request::Request(Request const &other) :
    _request_buffer(other._request_buffer),
    _request(other._request),
    _headers(other._headers),
    _method(other._method),
    _path(other._path),
    _http_version(other._http_version),
    _content_length(other._content_length),
    _body(other._body),
    _good_request(other._good_request),
    _is_ready(other._is_ready)
{}


// Getter for HTTP method
std::string Request::get_method() const
{
    return (this->_method);
}

// Getter for request path
std::string Request::get_path() const
{
    return (this->_path);
}

// Getter for specific header value by key
std::string Request::get_header_element(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end()) {
        return (it->second);
    }
    return (""); // Return empty string if header not found
}

// Getter for all the request header
std::string Request::get_all_header() const
{
    std::string result;
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
        result += it->first + ": " + it->second + "\r\n";
    }
    return (result);
}

std::map<std::string, std::string> Request::get_header() const
{
    return (this->_headers);
}

// Getter for request body
std::string Request::get_body() const
{
    return (this->_body);
}

bool Request::get_good_request() const
{
    return (this->_good_request);
}

int Request::get_is_ready() const
{
    return (this->_is_ready);
}

std::string  Request::get_request_buffer() const
{
    return (this->_request_buffer);
}
std::string  Request::get_http_version() const
{
    return (this->_http_version);
}

int Request::get_content_length() const
{
    return (this->_content_length);
}

// Method to set the HTTP method (used during request parsing)
void Request::set_method(const std::string& method)
{
    this->_method = method;
}

// Method to set the request path (used during request parsing)
void Request::set_path(const std::string& path)
{
    this->_path = path;
}

// Method to add a header (used during request parsing)
void Request::add_header(const std::string& key, const std::string& value)
{
    this->_headers[key] = value;
}

// Method to set the body (used during request parsing)
void Request::set_body(const std::string& body)
{
    this->_body = body;
}

void Request::set_good_request(const bool all_good)
{
    this->_good_request = all_good;
}

void Request::set_is_ready(const int ready)
{
    this->_is_ready = ready;
}

void Request::set_request_buffer(const std::string request_buffer)
{
    this->_request_buffer = request_buffer;
}

void Request::set_http_version(const std::string http_version)
{
    this->_http_version = http_version;
}

void Request::set_content_length(int lenght)
{
    this->_content_length = lenght;
}

void Request::set_to_null()
{
    this->_request_buffer = "";
    this->_request = "";
    this->_method = "";
    this->_path = "";
    this->_http_version = "";
    this->_content_length = 0;
    this->_body = "";
    this->_good_request = false;
    this->_is_ready = ENCORE;
}

Request::~Request() {}