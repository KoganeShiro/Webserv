#pragma once

#include "WebServ.hpp"

class Worker {
private:
    std::map<std::string, IHttpMethod*> _method_handlers;
   // Socket& _socket;
    Config_data _config;
    Request _request;
    Response _response;
    std::string _fullpath;
    std::string _location;
    std::string _file;

public:
    Worker(Socket& socket);

  //  Worker(Socket& socket, Server& server); // Constructor with pointer or reference to server to be able to access the configuration? Filepath etc. needed to execute requests.
    Worker(Config_data c, Request request); // Constructor with pointer or reference to server to be able to access the configuration? Filepath etc. needed to execute requests.

    Worker &Worker::operator=(const Worker &other);
    ~Worker();
    Response run();
//    void register_method(const std::string& methodName, IHttpMethod* handler);
//    void process_request(const Request& request);
    void send_response(const Response& response);
};