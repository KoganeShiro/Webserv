
#pragma once
#include "Config_data.hpp"
#include "Response.hpp"
#include "Request.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <map>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <dirent.h>
#include <sys/types.h>

//#include "WebServ.hpp"
class Response;
class Request;


/*
Interface to know which HTTP Method
*/
class IHttpMethod {
public:
    virtual Response handle(const Request& request, std::string& fullpath, Config_data c, std::string route) = 0;
    virtual ~IHttpMethod() {}
};
