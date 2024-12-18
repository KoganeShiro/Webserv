
#pragma once

#include "WebServ.hpp"

/*
Interface to know which HTTP Method
*/
class IHttpMethod {
public:
    virtual Response handle(const Request& request) = 0;
    virtual ~IHttpMethod() {}
};
