#pragma once

#include "GetMethod.hpp"
#include "PostMethod.hpp"
#include "DeleteMethod.hpp"

class Worker {
private:
    std::map<std::string, IHttpMethod*> _method_handlers;
   // Socket& _socket;
    Config_data _config;
    Request *_request;
    Response _response;
    std::string _fullpath;
    std::string _req_location;
    std::string _file;
    std::string _route;
    std::string _querystring;
    std::string _cgi_type;
    bool _use_cgi;
    int _status_code;
    bool _file_exists();
    bool _file_readable();
    bool _file_writable();
    void check_cgi();
    std::string checkRoute() const;
    bool _is_directory();
    void check_for_errors();
    bool is_valid_method();
    bool method_is_available();
    bool servername_is_valid();
    std::vector<std::string> build_cgi_environment();
    std::string execute_cgi();


public:
   // Worker(Socket& socket);

  //  Worker(Socket& socket, Server& server); // Constructor with pointer or reference to server to be able to access the configuration? Filepath etc. needed to execute requests.
    Worker(Config_data c, Request *request); // Constructor with pointer or reference to server to be able to access the configuration? Filepath etc. needed to execute requests.

 //   Worker &operator=(const Worker &other);
    ~Worker();
    Response run();
//    void register_method(const std::string& methodName, IHttpMethod* handler);
//    void process_request(const Request& request);
 //   void send_response(const Response& response);
};