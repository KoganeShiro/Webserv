
#include "Request.hpp"

static bool parse_request_line(Request& request, std::string& buffer, size_t& pos)
{
    size_t line_end = buffer.find("\n", pos);
    if (line_end != std::string::npos) {
        std::string first_line = buffer.substr(pos, line_end - pos);
        std::istringstream iss(first_line);
        std::string method, path, http_version;
        if (iss >> method >> path >> http_version) {
            request.set_method(method);
            request.set_path(path);
            request.set_http_version(http_version);
            pos = line_end + 1;
            return (true);
        }
    }
    return (false);
}

static bool parse_headers(Request& request, std::string& buffer, size_t& pos)
{
    size_t line_end;
    size_t start_pos = pos;

    std::cout << BLUE << buffer << RESET << std::endl;

    while ((line_end = buffer.find("\r\n", pos)) != std::string::npos) {
        if (pos == line_end) {
            pos += 2;
            return (true);
        }
        if (line_end - start_pos > MAX_HEADER_SIZE) {
            request.set_is_ready(BAD_HEADER);
            return (false);
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
            std::cout << key << std::endl;
            std::cout << value << std::endl;
            request.add_header(key, value);
        }
        pos = line_end + 2;
    }
    return (false);
}

static bool parse_body(Request& request, std::string& buffer, size_t& pos)
{
    if (request.get_header_element("Content-Length").empty() != false) {
        if (buffer.length() >= pos + request.get_content_length()) {
            request.set_body(buffer.substr(pos, request.get_content_length()));
            pos += request.get_content_length();
            request.set_is_ready(GOOD);
            return (true);
        }
    }
    else if (request.get_header_element("Transfer-Encoding") == "chunked") {
        //handle chunked
    }
    return (false);
}

static bool validate_headers(Request &request)
{
    std::string content_length = request.get_header_element("Content-Length");
    std::string transfer_encoding = request.get_header_element("Transfer-Encoding");
    if (content_length.empty() && transfer_encoding != "chunked") {
        request.set_is_ready(BAD_HEADER);
        std::cout << RED "header: no content-length or chunked" RESET << std::endl;
        return (false);
    }
    request.set_content_length(atoi(content_length.c_str()));
    return (true);
}

Request Request::request_parser(Request &request, std::string& buffer)
{
    size_t pos = 0;

    if (!parse_request_line(request, buffer, pos))
        return (request);
    if (!parse_headers(request, buffer, pos))
        return (request);
    if (!validate_headers(request))
        return (request);
    if (!parse_body(request, buffer, pos))
        return (request);

    request.set_request_buffer(buffer.substr(pos));

    std::cout << "reminder: " BGREEN <<
        request.get_request_buffer()
    << RESET << std::endl;

    request.set_good_request(true);
    request.set_is_ready(GOOD);

    return (request);
}

void Request::add_to_request(std::string to_add)
{
    this->set_good_request(false);
    this->set_is_ready(AGAIN);
    this->set_content_length(0);

    this->set_request_buffer(this->_request_buffer.append(to_add));
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
