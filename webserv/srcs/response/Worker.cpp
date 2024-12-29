
#include "WebServ.hpp"

//method_handler()
/*
Worker::Worker(Socket& socket) : _socket(socket)
{}
*/
Worker::Worker(Config_data c, Request request)
{
    _method_handlers["GET"] = new GetMethod();
    _method_handlers["POST"] = new PostMethod();
    _method_handlers["DELETE"] = new DeleteMethod();
    _config = c;
    _request = request;    
    _location = _request.get_path().substr(0,_request.get_path().find_last_of('/')); // Get the location of the file
    if (_location.empty()) {
        _location = "/";
    }
    _file = _request.get_path().substr(_request.get_path().find_last_of('/') + 1); // Get the file name
    _fullpath = _config.route_config->root_dir + _request.get_path();

    
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
}


Response Worker::run()
{
    Response response;
    _request.get_path();
    _config.routes["/"].accepted_methods( ["GET", "POST", "DELETE"]);




    std::string method = _request.get_method();  
    if (_method_handlers.find(method) != _method_handlers.end()) {
        IHttpMethod* handler = _method_handlers[method];
        response = handler->handle(_request);
    }
    else {
        response = Response(405, "Method Not Allowed");
    }
    
    return response;
    //send_response(response);
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