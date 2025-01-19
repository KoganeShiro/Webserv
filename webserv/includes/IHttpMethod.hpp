
#pragma once

#include "Cgi.hpp"
#include "Response.hpp"
#include "Request.hpp"

/*
Interface to know which HTTP Method
*/
class IHttpMethod {
public:
    virtual Response handle(const Request& request, std::string& fullpath, Config_data c, std::string route) = 0;
    virtual ~IHttpMethod() {}
};
