
#pragma once

#include "WebServ.hpp"
class Response;
class Request;


/*
Interface to know which HTTP Method
*/
class IHttpMethod {
public:
    virtual Response handle(const Request& request, std::string& fullpath) = 0;
    virtual ~IHttpMethod() {}
};
