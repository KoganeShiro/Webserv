
#include "WebServ.hpp"

//method_handler()

Worker::Worker(Socket& socket) : _socket(socket)
{}

void register_method(const std::string& method_name, IHttpMethod* handler)
{
    /*
    _method_handlers["GET"] = new GetMethod();
    _method_handlers["POST"] = new PostMethod();
    _method_handlers["DELETE"] = new DeleteMethod();
    */
    _method_handlers[method_name] = handler; // Register HTTP method handler
}

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

void Worker::send_response(const Response& response)
{
    std::string response.set_body(gen_htmlbody());
    std::string responseStr = response.http_response();
    ssize_t bytesSent = send(this->_socket->sockfd, fullResponse.c_str(),
            fullResponse.length(), 0);
    if (bytesSent < 0) {
        perror("Failed to send response");
        close(); // Close connection on error
    }
}
