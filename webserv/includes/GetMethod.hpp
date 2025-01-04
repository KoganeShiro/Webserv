#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

class Response;

/*
GET: Retrieves a resource
*/
class GetMethod : public IHttpMethod {
private:   
    Config_data _config;
    Request _request;
    Response _response;
    std::string _fullpath;
  //  std::string _req_location;
  //  std::string _file;
    std::string _route;
    int _status_code;
    bool _file_exists();
    bool _file_readable();
    bool _file_writable();
  //  bool _is_cgi();
  //  std::string checkRoute() const;
    bool _is_directory();
   // void check_for_errors();
  //  bool is_valid_method();
  //  bool method_is_available();
  //  bool servername_is_valid();
  //  std::string cgi_type();
  std::string readfile(std::string filename);
  int filesize(std::string filename);
  std::string getMimeType(const std::string& fileName);




public:
    Response handle(const Request& request, std::string& fullpath, Config_data c, std::string route);
};