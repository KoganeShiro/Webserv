
#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

/*
POST: Submits data to be processed
*/
class PostMethod : public IHttpMethod {
public:
    Response handle(const Request& request) override {
        // Handle POST request
        // Process submitted data
        // Return Response object
    }
};