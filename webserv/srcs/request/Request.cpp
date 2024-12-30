
#include "WebServ.hpp"

Request::Request() {}

Request::Request(Request const &other) :
    _request_buffer(other._request_buffer),
    _request(other._request), _headers(other._headers),
    _method(other._method), _path(other._path),
    _content_length(other._content_lenght),
    _body(other._body), _good_request(other._good_request),
    _is_ready(other._is_ready), _http_version(other._http_version)
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
    auto it = _headers.find(key);
    if (it != _headers.end()) {
        return (it->second);
    }
    return (""); // Return empty string if header not found
}

// Getter for all the request header
// std::string Request::get_header() const
// {
//     return (this->_header_str);
// }

// Getter for request body
std::string Request::get_body() const
{
    return (this->_body);
}

bool Request::get_good_request() const
{
    return (this->_good_request);
}

bool Request::get_is_ready() const
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

void Request::set_is_ready(const bool ready)
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
    this->_is_ready = false;
}

static bool parse_request_line(Request& request, std::string& buffer, size_t& pos)
{
    size_t line_end = buffer.find("\r\n", pos);
    if (line_end != std::string::npos) {
        std::string first_line = buffer.substr(pos, line_end - pos);
        std::istringstream iss(first_line);
        std::string method, path, http_version;
        if (iss >> method >> path >> http_version) {
            request.set_method(method);
            request.set_path(path);
            request.set_http_version(http_version);
            pos = line_end + 2;
            return (true);
        }
    }
    return (false);
}

static void parse_headers(Request& request, std::string& buffer, size_t& pos)
{
    size_t line_end;
    while ((line_end = buffer.find("\r\n", pos)) != std::string::npos) {
        if (pos == line_end) {
            break;
        }

        std::string line = buffer.substr(pos, line_end - pos);
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value.erase(0, 1);
            while (!value.empty() && (value[value.size() - 1] == ' ' || value[value.size() - 1] == '\t'))
                value.erase(value.size() - 1);
            request.add_header(key, value);
        }
        pos = line_end + 2;
    }
}

static void parse_body(Request& request, std::string& buffer, size_t& pos)
{
    if (buffer.length() >= pos + request.get_content_length()) {
        request.set_body(buffer.substr(pos, request.get_content_length()));
        pos += request.get_content_length();
        request.set_is_ready(true);
    }
}

static Request request_parser(Request request, std::string& buffer)
{
    size_t pos = 0;

    if (request.get_method().empty()) {
        if (!parse_request_line(request, buffer, pos)) {
            return request;
        }
    }
    size_t headers_end = buffer.find("\r\n\r\n", pos);
    if (headers_end == std::string::npos) {
        return request;
    }
    parse_headers(request, buffer, pos);

    request.set_content_length(atoi(request.get_header_element("Content-Length").c_str()));
    if (request.get_content_length() > 0) {
        if (buffer.length() < headers_end + 4 + request.get_content_length()) {
            return request;
        }
        parse_body(request, buffer, pos);
    }
    else {
        pos = headers_end + 4;
    }

    buffer = buffer.substr(pos);

    request.set_good_request(true);
    request.set_is_ready(true);
    return (request);
}


void Request::add_to_request(std::string to_add)
{
    this.set_good_request(false);
    this->set_is_ready(false);
    this->set_content_length(0);

    this->_request_buffer = this->_request_buffer.append(to_add);
    *this = request_parser(*this, this->_request_buffer);
}

Request *Request::parsed_request()
{
    std::string request_buffer;

    std::string request_buffer = this->get_request_buffer();
    Request *ready = new Request(*this);
    this->set_to_null();
    this->set_request_buffer(request_buffer);
    return (ready);
}