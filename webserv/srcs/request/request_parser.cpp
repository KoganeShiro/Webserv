
#include "Request.hpp"
void Request::set_finish_header(bool finish)
{
    this->_finish_header = finish;
}

bool Request::get_finish_header()
{
    return (this->_finish_header);
}

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

    while ((line_end = buffer.find("\r\n", pos)) != std::string::npos) {
        if (pos == line_end) {
            pos += 2;
            request.set_pos(pos);
            request.set_finish_header(true);
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
            request.add_header(key, value);
        }
        pos = line_end + 2;
    }
    return (false);
}

static bool validate_headers(Request &request, size_t MAX_BODY_LENGTH)
{
    std::string content_length = request.get_header_element("Content-Length");
    std::string transfer_encoding = request.get_header_element("Transfer-Encoding");
    if (content_length.empty() && transfer_encoding != "chunked") {
        if (request.get_method() == "GET" || request.get_method() == "DELETE") {
            return (true);
        }
        request.set_is_ready(BAD_HEADER);
        return (false);
    }
    request.set_content_length(atoi(content_length.c_str()));
    if (request.get_content_length() > MAX_BODY_LENGTH) {
        request.set_is_ready(BAD_HEADER);
        return (false);
    }
    return (true);
}

static bool parse_body(Request& request, std::string& buffer, size_t MAX_BODY_LENGTH)
{
    size_t pos = request.get_pos();
    if (request.get_header_element("Content-Length").empty() == false) {
        if (buffer.length() >= pos + request.get_content_length()) {
            request.set_body(buffer.substr(pos, request.get_content_length()));
            pos += request.get_content_length();
            request.set_is_ready(GOOD);
            return (true);
        }
    }
    else if (request.get_header_element("Transfer-Encoding") == "chunked") {
         while (true) {
            size_t chunk_size_end = buffer.find("\r\n", pos);
            if (chunk_size_end == std::string::npos) {
                return (false);
            }
            std::string chunk_size_str = buffer.substr(pos, chunk_size_end - pos);
            pos = chunk_size_end + 2;
            size_t chunk_size;
            std::istringstream iss(chunk_size_str);
            if (!(iss >> std::hex >> chunk_size)) {
                request.set_is_ready(BAD_HEADER);
                return (false);
            }
            if (chunk_size == 0) {
                request.set_is_ready(GOOD);
                pos = buffer.find("\r\n", pos) + 2;
                request.set_pos(pos);
                return (true);
            }
            if (buffer.length() < pos + chunk_size + 2) {
                return (false);
            }
            request.set_body(request.get_body() + buffer.substr(pos, chunk_size));
            pos += chunk_size + 2;
            request.set_pos(pos);
            if (request.get_body().length() > MAX_BODY_LENGTH) {
                request.set_is_ready(BAD_HEADER);
                return (false);
            }
        }
    }
    request.set_pos(pos);
    return (false);
}

static std::string extract_boundary(const std::string& content_type)
{
    std::string boundary_prefix = "boundary=";
    size_t boundary_pos = content_type.find(boundary_prefix);
    if (boundary_pos != std::string::npos) {
        return ("--" + content_type.substr(boundary_pos + boundary_prefix.length()));
    }
    return ("");
}

static bool handle_multipart_form_data(Request& request)
{
    std::string content_type = request.get_header_element("Content-Type");
    std::string boundary = extract_boundary(content_type);
    std::string buffer = request.get_body();
    if (boundary.empty()) {
        request.set_is_ready(BAD_HEADER);
        return (false);
    }
    std::string body;
    size_t start = 0, next_boundary;
    while (true) {
        next_boundary = buffer.find(boundary, start);
        if (next_boundary == std::string::npos) {
            break;
        }
        if (next_boundary > start) {
            size_t colon_start = buffer.find(":", start);
            size_t end = buffer.find("\r\n", colon_start);
            request.add_header("Content-Disposition", buffer.substr(colon_start + 2, end - colon_start - 2));
            //next line
            start = buffer.find(":", end);
            end = buffer.find("\r\n", start);
            request.add_header("Content-Type", buffer.substr(start + 2, end - start - 2));
            //next line
            start = buffer.find("\r\n", end);
            end = buffer.find("\r\n\r\n", start);
            body += buffer.substr(start + 2, next_boundary - start - 2);
        }
        start = next_boundary + boundary.length();
        size_t end = buffer.find("\r\n", start);
        if (end == std::string::npos) {
            break;
        }
        start = end + 2;
    }
    request.set_body(body);
    request.set_is_ready(GOOD);
    return (true);
}

Request Request::request_parser(Request &request, std::string& buffer, size_t MAX_BODY_LENGTH)
{
    size_t pos = 0;

    if (!parse_request_line(request, buffer, pos))
        return (request);
    if (!request.get_finish_header()) {
        if (!parse_headers(request, buffer, pos))
            return (request);
    }
    if (!validate_headers(request, MAX_BODY_LENGTH))
        return (request);
    if (request.get_method() == "POST") {
        // std::string content_type = request.get_header_element("Content-Type");
        // if (content_type.find("multipart/form-data;") != std::string::npos) {
        //     if (!handle_multipart_form_data(request, buffer, pos)) {
        //         std::cout << MAGENTA "multipart/form-data; / false" RESET << std::endl;
        //         return (request);
        //     }
        // } else {
            if (!parse_body(request, buffer, MAX_BODY_LENGTH))
                return (request);
        //}
    }

    std::string content_type = request.get_header_element("Content-Type");
    if (content_type.find("multipart/form-data;") != std::string::npos) {
        if (!handle_multipart_form_data(request)) {
            std::cout << MAGENTA "multipart/form-data; / false" RESET << std::endl;
            return (request);
        }
    }

    request.set_request_buffer(buffer.substr(request.get_pos()));
    std::cout << MAGENTA "request_buffer: " << request.get_request_buffer()
    << RESET << std::endl;

    std::cout << "reminder: " << request.get_request_buffer() << std::endl;

    request.set_good_request(true);
    request.set_is_ready(GOOD);

    return (request);
}

void print_Request(Request *request)
{
    if (request == NULL)
    {
        std::cout <<RED "Request is NULL" RESET << std::endl;
        return;
    }
    std::cout << CYAN "-----------------------" << std::endl;
    std::cout << "Request Details:" << std::endl;
    std::cout << "Request Buffer: `" << request->get_request_buffer() << "`" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Method: `" << request->get_method() << "`" << std::endl;
    std::cout << "Path: `" << request->get_path() << "`" << std::endl;
    std::cout << "HTTP Version: `" << request->get_http_version() << "`" << std::endl;
    std::cout << "Content Length: `" << request->get_content_length() << "`" << std::endl;
    std::cout << "Good Request: `" << (request->get_good_request() ? "Yes" : "No") << "`" << std::endl;
    std::cout << "Is Ready: `" << (request->get_is_ready() ? "Yes" : "No") << "`" << std::endl;

    std::cout << "\nHeaders:" << std::endl;
    std::cout << "--------" << std::endl;
    // Assuming you have a method to get all headers
    std::map<std::string, std::string> headers = request->get_header();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << "`" << it->first << "`" << ": "
         << "`" << it->second << "`" << std::endl;
    }

    std::cout << "\nBody:" << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << "`" << request->get_body() << "`" << std::endl;
    std::cout << "-----------------------" RESET << std::endl;
}

int Request::add_to_request(std::string to_add, size_t MAX_BODY_LENGTH)
{
    this->set_good_request(false);
    this->set_is_ready(AGAIN);

    this->set_request_buffer(this->_request_buffer.append(to_add));
    *this = request_parser(*this, this->_request_buffer, MAX_BODY_LENGTH);
    print_Request(this);
    return (this->get_is_ready());
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