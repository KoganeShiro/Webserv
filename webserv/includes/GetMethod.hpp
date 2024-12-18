
#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

/*
GET: Retrieves a resource
*/
class GetMethod : public IHttpMethod {
public:
    Response handle(const Request& request) override {
        // Handle GET request
        // Read file or generate content
        // Return Response object
    }
};