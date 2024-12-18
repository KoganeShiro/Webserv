
#include "WebServ.hpp"

// Getter for HTTP method
std::string get_method() const
{
    return (this->_method);
}

// Getter for request path
std::string get_path() const
{
    return (_path);
}

// Getter for specific header value by key
std::string get_header_element(const std::string& key) const
{
    auto it = _headers.find(key);
    if (it != _headers.end()) {
        return (it->second);
    }
    return (""); // Return empty string if header not found
}

// Getter for all the request header
std::string get_header_element(const std::string& key) const
{
    return (this->_header_str);
}

// Getter for request body
std::string get_body() const
{
    return (_body);
}

// Method to set the HTTP method (used during request parsing)
void set_method(const std::string& method)
{
    this->_method = method;
}

// Method to set the request path (used during request parsing)
void set_path(const std::string& path)
{
    this->_path = path;
}

// Method to add a header (used during request parsing)
void add_header(const std::string& key, const std::string& value)
{
    this->_headers[key] = value;
}

// Method to set the body (used during request parsing)
void set_body(const std::string& body)
{
    this->_body = body;
}

