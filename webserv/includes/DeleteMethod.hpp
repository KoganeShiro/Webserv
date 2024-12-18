
#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

/*
DELETE: Removes a specified resource
*/
class DeleteMethod : public IHttpMethod {
public:
    Response handle(const Request& request) override {
        // Handle DELETE request
        // Delete resource if exists and permitted
        // Return Response object
    }
};
