
//#include "WebServ.hpp"

#include "Worker.hpp"

//method_handler()
/*
Worker::Worker(Socket& socket) : _socket(socket)
{}
*/

std::string Worker::cgi_type()
{
    return (_config.routes[_route].cgi_extension);
}

bool Worker::_is_cgi()
{
    if (_config.routes[_route].cgi_extension != "") {
        if (_fullpath.rfind(_config.routes[_route].cgi_extension) == _fullpath.length() - _config.routes[_route].cgi_extension.length()) {
            return true;
        }
    }
    return false;
}

bool Worker::_file_exists()
{
    struct stat buffer;
    return (stat (_fullpath.c_str(), &buffer) == 0);
}

bool Worker::_file_readable()
{
    if (access(_fullpath.c_str(), R_OK) == 0) {
        return true;
    }
    return false;
}

bool Worker::_file_writable()
{
        if (access(_fullpath.c_str(), W_OK) == 0) {
            return true;
        }
        return false;
}

std::string Worker::checkRoute() const
{
    std::cout << "CheckRoute: Request path: " << _request->get_path() << std::endl;
    std::string route = "";
    long unsigned int length = 0;
    for (std::map< std::string, Route_config >::const_iterator it = _config.routes.begin(); it != _config.routes.end(); ++it) {
        if ((_request->get_path()).rfind(it->first, 0) == 0)
        {
            if ((it->first).length() > length) {
                route = it->first;
                length = route.length();
            }
        }
    }
    std::cout << "CheckRoute: Route: " << route << std::endl;
    return route;
}

bool Worker::method_is_available()
{
    for (std::map<std::string, IHttpMethod*>::iterator it = _method_handlers.begin(); it != _method_handlers.end(); ++it) {
        if (it->first == _request->get_method()) {
            std::cout << "Method available" << std::endl;
            return true;
        }
    }
    std::cout << "Method not available" << std::endl;
    return false;
}

bool Worker::is_valid_method()
{
    for (std::vector<std::string>::iterator it = _config.routes[_route].accepted_methods.begin(); it != _config.routes[_route].accepted_methods.end(); ++it) {
        if (*it == _request->get_method()) {
            return true;
        }
    }
    return false;
}

bool Worker::servername_is_valid()
{
    for (std::vector<std::string>::iterator it = _config.server_names.begin(); it != _config.server_names.end(); ++it) {
        if (*it == _request->get_header_element("Host")) {
            return true;
        }
    }
    return false;
}

bool Worker::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}

void Worker::check_for_errors()
{
    if (! is_valid_method() || ! method_is_available()) {
        _status_code = 405;
        std::cout << "Method not allowed" << std::endl;
        return;
    }
    if (! servername_is_valid()) {
        _status_code = 404;
        std::cout << "Server name not found" << std::endl;
        return;
    }
    if (_route.empty()) {
        _status_code = 404;
        std::cout << "Route not found" << std::endl;
        return;
    }
/*    if (_config.routes[_route].redirection != "") {
        _status_code = 301;
        return;
    }
*/
/*  if (!_file_exists()) {
        _status_code = 404;
        return;
    }
    if (!_file_readable()) {
        _status_code = 403;
        return;
    }
    if (_is_directory() && !_config.routes[_route].dir_listing) {
        _status_code = 403;
        return;
    }
    _status_code = 200;
*/
}

Worker::Worker(Config_data c, Request *request)
{
    _status_code = 0;
    _method_handlers["GET"] = new GetMethod();
  //  _method_handlers["POST"] = new PostMethod();
  //  _method_handlers["DELETE"] = new DeleteMethod();
    _config = c;
    _request = request;
    _route = checkRoute();    
    if (! _route.empty())
    {
        /*
        std::cout << "Route: " << _route << std::endl;
        std::cout << "Root dir: " << _config.routes[_route].root_dir << std::endl;
        std::cout << "Request path: " << _request->get_path() << std::endl;
        std::cout << "Request path length: " << _request->get_path().length() << std::endl;
        std::cout << "Route length: " << _route.length() << std::endl;
        std::cout << "Request path substr: " << _request->get_path().substr(_route.length()) << std::endl;
*/

        _fullpath = (_config.routes[_route].root_dir + _request->get_path()).substr(1);
        if (_fullpath[_fullpath.length() - 1] == '/')
           _fullpath += _config.routes[_route].default_file;
    }
    else
        _status_code = 404;
//    _file = _fullpath.substr(_fullpath.find_last_of('/') + 1); // Get the file name
    std::cout << "Fullpath: " << _fullpath << std::endl;
    check_for_errors();

    
}
/*
void Worker::register_method(const std::string& method_name, IHttpMethod* handler)
{

    _method_handlers["GET"] = new GetMethod();
    _method_handlers["POST"] = new PostMethod();
    _method_handlers["DELETE"] = new DeleteMethod();

    _method_handlers[method_name] = handler; // Register HTTP method handler
}
*/

Worker::~Worker()
{
    for (std::map<std::string, IHttpMethod*>::iterator it = _method_handlers.begin(); it != _method_handlers.end(); ++it) {
        delete it->second;
    }
    delete _request;
}


Response Worker::run()
{
    Response response;
    if (_status_code > 399) {
        response = Response(_status_code, "Error", _config);
        std::cout << "Error reponse generated from Worker: " << _status_code << std::endl;
        return response;
    }    
 /*   if (_is_cgi()) {
        Cgi cgi(_config, _request, _fullpath, cgi_type());
        response = cgi.run();
        return response;
    }
*/
    std::cout << "Method: " << _request->get_method() << std::endl;
    response = _method_handlers[_request->get_method()]->handle(*_request, _fullpath, _config, _route);
    return response;    
}

/*

void Worker::process_request(const Request& request)
{
    Response response;
    std::string method = request.get_method(); 
    
    if (_method_handlers.find(method) != _method_handlers.end()) {
        IHttpMethod* handler = _method_handlers[method];
        response = handler->handle(request);
    }
    else {
        response = Response(405, "Method Not Allowed");
    }
    
    send_response(response);
}
*/
/*
void Worker::send_response(const Response& response)
{
    response.set_body(gen_htmlbody());
    std::string fullResponse = response.http_response();
    ssize_t bytesSent = send(this._socket->_sockfd, fullResponse.c_str(),
            fullResponse.length(), 0);
    if (bytesSent < 0) {
        perror("Failed to send response");
        close(); // Close connection on error
    }
}
*/