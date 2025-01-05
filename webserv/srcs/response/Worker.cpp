
//#include "WebServ.hpp"

#include "Worker.hpp"

//method_handler()
/*
Worker::Worker(Socket& socket) : _socket(socket)
{}
*/

void Worker::check_cgi()
{
    _use_cgi = false;
    if (_config.routes[_route].use_cgi)
    {
        size_t start_query_str = _fullpath.find('?');
        if (start_query_str != std::string::npos)
        {
            _querystring = _fullpath.substr(start_query_str + 1);
            _fullpath = _fullpath.substr(0,start_query_str);
        }
        std::string file_extension = _fullpath.substr(_fullpath.find_last_of('.') + 1);
        std::cout << "File extension: " << file_extension << std::endl;
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {
            std::cout << "Iterator CGI type: " << it->get_extension() << std::endl;
            if (it->get_extension() == file_extension) {
                _cgi_type = it->get_name();
                _use_cgi = true;
                break;
            }
        }
//        if (_cgi_type.empty()) {
//            _status_code = 415;
//        }
    }
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
    if (_config.server_name == _request->get_header_element("Host")) {
        return true;
    }
   // for (std::vector<std::string>::iterator it = _config.server_names.begin(); it != _config.server_names.end(); ++it) {
     //   if (*it == _request->get_header_element("Host")) {
      //      return true;
      //  }
   // }
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
    if (_config.routes[_route].redirection_nb > 299 && _config.routes[_route].redirection_path != "") {
        _status_code = _config.routes[_route].redirection_nb;
        std::cout << "Redirection" << std::endl;
        return;
    }

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
    _method_handlers["POST"] = new PostMethod();
    _method_handlers["DELETE"] = new DeleteMethod();
    _config = c;
    _request = request;
    _route = checkRoute();

    if (! _route.empty())
    {
        _fullpath = (_config.routes[_route].root_dir + _request->get_path());
        if (_fullpath[_fullpath.length() - 1] == '/')
           _fullpath += _config.routes[_route].default_file;
    }
    else
        _status_code = 404;
//    _file = _fullpath.substr(_fullpath.find_last_of('/') + 1); // Get the file name
    check_cgi();
    std::cout << "Fullpath: " << _fullpath << std::endl;
    std::cout << "Query String: " << _querystring << std::endl;
    std::cout << "CGI Type: " << _cgi_type << std::endl;
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
    if (_status_code > 299) {
        response = Response(_status_code, "Redirect", _config);
        response.set_header("Location", _config.routes[_route].redirection_path);
        std::cout << "Redirect reponse generated from Worker: " << _status_code << std::endl;
        return response;
    }    
    if (_use_cgi) {
        // response = run cgi (returns response (ststus code, message and body (with header)
        std::cout << "Running CGI" << std::endl;
        response = Response(200, "OK", execute_cgi());
        return response;        
    }
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



void Worker::build_cgi_environment(std::vector<char*>& envp)
{
    std::cout << "Building CGI environment" << std::endl;
    // Add environment variables directly to envp

    std::cout << const_cast<char*>(("REQUEST_METHOD=" + _request->get_method()).c_str()) << std::endl;

    envp.push_back(const_cast<char*>(("REQUEST_METHOD=" + _request->get_method()).c_str()));
    envp.push_back(const_cast<char*>(("QUERY_STRING=" + _querystring).c_str()));
    
    // Add headers as environment variables if they exist
    if (_request->get_header_element("Content-Length") != "") {
        envp.push_back(const_cast<char*>(("CONTENT_LENGTH=" + _request->get_header_element("Content-Length")).c_str()));
    }
    if (_request->get_header_element("Content-Type") != "") {
        envp.push_back(const_cast<char*>(("CONTENT_TYPE=" + _request->get_header_element("Content-Type")).c_str()));
    }
    std::ostringstream oss;
    oss << _config.port;
    std::string port = oss.str();
    envp.push_back(const_cast<char*>(("SCRIPT_NAME=" + _fullpath).c_str()));
    envp.push_back(const_cast<char*>(("SERVER_NAME=" + _config.server_name).c_str()));
    envp.push_back(const_cast<char*>(("SERVER_PORT=" + port).c_str()));
    // Null-terminate the array (as required by execve)
    envp.push_back('\0');

std::cout << "Environment variables:" << std::endl;
    for (std::vector<char*>::iterator it = envp.begin(); it != envp.end(); ++it) {
        std::cout << *it << std::endl;
    }



}

std::string Worker::execute_cgi() {
    int pipefd[2];
    pipe(pipefd);
    std::string result;

    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "Child process" << std::endl;
        // Child process: Execute CGI script
        // Build the environment for execve
        std::vector<char*> envp;
        build_cgi_environment(envp);


        // Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);               // Close unused read end
        
        // Execute the script with the custom environment and arguments
        std::cout << "Executing CGI script" << std::endl;
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {
            if (it->get_name() == _cgi_type) {
                const char *argv[] = { it->get_path().c_str(), NULL};
                if (execve(it->get_path().c_str(), (char *const *)argv, envp.data()) == -1) {
                    perror("execve failed");  // If execve fails
                    exit(1);
                }
            }
        }
    } else {
        std::cout << "Parent process" << std::endl;
        // Parent process: Read script output
        close(pipefd[1]); // Close unused write end
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            result.append(buffer, bytes_read);
            std::cout << "Read " << bytes_read << " bytes: " << buffer << std::endl;
        }
        close(pipefd[0]);
        waitpid(pid, NULL, 0); // Wait for child process to finish
    }
    return result;
}