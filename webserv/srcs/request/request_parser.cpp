
#include "WebServ.hpp"
#define HEADER_SIZE 8000

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
        request.set_is_ready(GOOD);
    }
}

static Request request_parser(Request request, std::string& buffer)
{
    size_t pos = 0;

    if (request.get_method().empty()) {
        if (!parse_request_line(request, buffer, pos)) {
            return (request);
        }
    }
    size_t headers_end = buffer.find("\r\n\r\n", pos);
    if (headers_end == std::string::npos) {
        //check sizeof header
        if (sizeof(buffer) > HEADER_SIZE) {
            request.set_is_ready(BAD_HEADER)
        }
        return (request);
    }
    parse_headers(request, buffer, pos);

    //check if not Content-Length, chunked request ?...
    //if neither of them return BAD_HEADER
    request.set_content_length(atoi(request.get_header_element("Content-Length").c_str()));
    if (request.get_content_length() > 0) {
        if (buffer.length() < headers_end + 4 + request.get_content_length()) {
            return (request);
        }
        parse_body(request, buffer, pos);
    }
    else {
        pos = headers_end + 4;
    }

    buffer = buffer.substr(pos);

    request.set_good_request(true);
    request.set_is_ready(GOOD);
    return (request);
}


void Request::add_to_request(std::string to_add)
{
    this->set_good_request(false);
    this->set_is_ready(ENCORE);
    this->set_content_length(0);

    this->_request_buffer = this->_request_buffer.append(to_add);
    *this = request_parser(*this, this->_request_buffer);
}

Request *Request::parsed_request()
{
    std::string request_buffer;

    request_buffer = this->get_request_buffer();
    Request *ready = new Request(*this);
    this->set_to_null();
    this->set_request_buffer(request_buffer);
    return (ready);
}
