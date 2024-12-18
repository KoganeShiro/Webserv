
#pragma once

#include "WebServ.hpp"

class Worker {
private:
    std::map<std::string, IHttpMethod*> _method_handlers;
    Socket& _socket;

public:
    Worker(Socket& socket);
    ~Worker();
    void register_method(const std::string& methodName, IHttpMethod* handler);
    void process_request(const Request& request);
    void send_response(const Response& response);
};
