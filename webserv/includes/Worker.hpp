#pragma once

#include "GetMethod.hpp"
#include "PostMethod.hpp"
#include "DeleteMethod.hpp"

class Worker {
private:
    std::map<std::string, IHttpMethod*> _method_handlers;
    Config_data _config;
    Request *_request;
    Response _response;
    std::string _fullpath;
    std::string _req_location;
    std::string _route;
    std::string _querystring;
    std::string _cgi_type;
    std::string _cgi_path;
    int _cgi_timeout;
    
    bool _use_cgi;
    int _status_code;
    std::string _error_message;
    void check_cgi();
    std::string checkRoute() const;
    void check_for_errors();
    bool is_valid_method();
    bool method_is_available();
    bool servername_is_valid();
    std::vector<std::string> build_cgi_environment();
    Response execute_cgi();
    void clean_up();


public:
    Worker(Config_data c, Request *request); // Constructor with pointer or reference to server to be able to access the configuration? Filepath etc. needed to execute requests.

    ~Worker();
    Response run();
};