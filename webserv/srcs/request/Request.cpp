#include "Request.hpp"

Request::Request() :_request_buffer(""),
    _method(""),
    _path(""),
    _http_version(""),
    _content_length(0),
    _body(""),
    _good_request(false),
    _is_ready(AGAIN),
    _pos(0), _finish_header(false), _header_last_line("")
{}

Request::Request(Request const &other) :
    _request_buffer(other._request_buffer),
    _headers(other._headers),
    _method(other._method),
    _path(other._path),
    _http_version(other._http_version),
    _content_length(other._content_length),
    _body(other._body),
    _good_request(other._good_request),
    _is_ready(other._is_ready),
     _pos(other._pos), _finish_header(other._finish_header), _header_last_line(other._header_last_line)
{}

Request &Request::operator=(Request const &other)
{
	if (this != &other) {
		_request_buffer = other._request_buffer;
        _headers = other._headers;
        _path = other._path;
        _http_version = other._http_version;
        _content_length = other._content_length;
        _body = other._body;
        _good_request = other._good_request;
        _is_ready = other._is_ready;
	}
	return (*this);
}

std::string Request::get_method() const
{
    return (this->_method);
}

std::string Request::get_path() const
{
    return (this->_path);
}

std::string Request::get_header_element(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end()) {
        return (it->second);
    }
    return (""); // Return empty string if header not found
}

/*
std::string Request::get_all_header() const
{
    std::string result;
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
        result += it->first + ": " + it->second + "\r\n";
    }
    return (result);
}
*/

std::map<std::string, std::string> Request::get_header() const
{
    return (this->_headers);
}


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

size_t Request::get_content_length() const
{
    return (this->_content_length);
}

size_t Request::get_pos() const
{
    return (this->_pos);
}

std::string Request::get_header_last_line() const
{
    return (this->_header_last_line);
}

void Request::set_header_last_line(const std::string header_last_line)
{
    this->_header_last_line = header_last_line;
}

void Request::set_pos(size_t pos)
{
    this->_pos = pos;
}

void Request::set_method(const std::string& method)
{
    this->_method = method;
}

void Request::set_path(const std::string& path)
{
    this->_path = path;
}

void Request::add_header(const std::string& key, const std::string& value)
{
    this->_headers[key] = value;
}

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

void Request::set_content_length(size_t length)
{
    this->_content_length = length;
}

void Request::set_to_null()
{
    this->_request_buffer = "";
    this->_method = "";
    this->_path = "";
    this->_http_version = "";
    this->_content_length = 0;
    this->_body = "";
    this->_good_request = false;
    this->_is_ready = AGAIN;
}

Request::~Request() {}