
#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

#define ERROR -1
#define NO_FILENAME -2

//class Response;

/*
POST: Submits data to be processed

class PostMethod : public IHttpMethod {
public:
    Response handle(const Request& request) override {
        // Handle POST request
        // Process submitted data
        // Return Response object
    }
};
*/

class PostMethod : public IHttpMethod {
private:   
    Config_data _config;
    Request _request;
    Response _response;
    std::string _fullpath;
  //  std::string _req_location;
  //  std::string _file;
    std::string _route;
    std::string _content;
    std::string _content_type;
    std::string _filename;
    std::string _full_filename;
    std::string _multipath_boundary;
    std::string _content_disposition;
    
    std::string next_part;

    int _status_code;
    bool _file_exists();
    bool _file_readable();
    bool _file_writable();
    void _set_filename();
  //  void _set_content_type();
  //  void _set_content();
    void _set_multipath_boundary();
    void _upload_file(std::string &content, std::string &filename);
    void _parse_body(std::string &content, std::string &filename);
  //  void _set_content_disposition();
  //  void next_form_data();
  //  void header_form_data_part();
    void _extract_next_part();
    void _extract_content();
    void _set_filename_and_content();

  //  bool _is_cgi();
  //  std::string checkRoute() const;
    bool _is_directory();
   // void check_for_errors();
  //  bool is_valid_method();
  //  bool method_is_available();
  //  bool servername_is_valid();
  //  std::string cgi_type();
  // std::string readfile(std::string filename);
  int writefile(std::string filename, std::string content);
  int filesize(std::string filename);
  std::string getMimeType(const std::string& fileName);


public:
    Response handle(const Request& request, std::string& fullpath, Config_data c, std::string route);
};