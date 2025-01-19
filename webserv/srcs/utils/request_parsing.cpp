#include "Request.hpp"

std::string extract_boundary(const std::string& content_type)
{
    std::string boundary_prefix = "boundary=";
    size_t boundary_pos = content_type.find(boundary_prefix);
    if (boundary_pos != std::string::npos) {
        return ("--" + content_type.substr(boundary_pos + boundary_prefix.length()));
    }
    return ("");
}

std::string clean_request_body(const std::string& buffer)
{
    std::string body = buffer;
    std::string::size_type pos;

    // Remove Content-Disposition line
    pos = body.find("Content-Disposition");
    if (pos != std::string::npos) {
        body = body.substr(0, pos) + body.substr(body.find("\r\n", pos) + 2);
    }
    // Remove Content-Type line
    pos = body.find("Content-Type: ");
    if (pos != std::string::npos) {
        body = body.substr(0, pos) + body.substr(body.find("\r\n", pos) + 2);
    }
    // Remove line break
    pos = body.find("\r\n");
    if (pos != std::string::npos) {
        body = body.substr(0, pos) + body.substr(pos + 2);
    }
    return (body);
}