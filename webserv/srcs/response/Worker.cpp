
//#include "WebServ.hpp"

#include "Worker.hpp"

void Worker::check_cgi()
{
    std::cout << YELLOW "Check if CGI is available and parse information from Path." RESET << std::endl;
    if (_config.routes[_route].use_cgi)
    {
        size_t start_query_str = _fullpath.find('?');
        if (start_query_str != std::string::npos)
        {
            _querystring = _fullpath.substr(start_query_str + 1);
            _fullpath = _fullpath.substr(0,start_query_str);
        }
        std::string extension_path_query = _fullpath.substr(_fullpath.find_last_of('.') + 1);        
        if (extension_path_query.find('/') != std::string::npos) {
            _cgi_path = extension_path_query.substr(extension_path_query.find('/'));            
            extension_path_query = extension_path_query.substr(0, extension_path_query.find('/'));
            _fullpath = _fullpath.substr(0, _fullpath.find_last_of('.') + 1) + extension_path_query;
        }                
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {   
            std::cout << YELLOW "Checking CGI type: " RESET << it->get_name() << std::endl;         
            if (it->get_extension() == extension_path_query) {
                _cgi_type = it->get_name();
                _use_cgi = true;
                _cgi_timeout = it->get_time_out();
                std::cout << ORANGE "CGI type: " RESET << _cgi_type << std::endl;
                std::cout << ORANGE "CGI Path_Info: " RESET << _cgi_path << std::endl;
                std::cout << ORANGE "CGI Query_string: " RESET << _querystring << std::endl;
                std::cout << ORANGE "Path to CGI Script: " RESET << _fullpath << std::endl;
                break;
            }
        }
    }
    if (! _use_cgi)
        std::cout << ORANGE "CGI not available or non CGI file extension." RESET << std::endl;
}
/*
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
*/

std::string Worker::checkRoute() const
{
    
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
    if (route.empty())
        std::cout << RED "CheckRoute: " << _request->get_path() << " maps to no route" << RESET << std::endl;
    else
        std::cout << GREEN "CheckRoute: " << _request->get_path() << " maps to " << route << RESET << std::endl;
    return route;
}

bool Worker::method_is_available()
{
    for (std::map<std::string, IHttpMethod*>::iterator it = _method_handlers.begin(); it != _method_handlers.end(); ++it) {
        if (it->first == _request->get_method()) {
            std::cout << GREEN "Method " << _request->get_method() << " is available in this Worker service." RESET << std::endl;
            return true;
        }
    }
    std::cout << RED "Method " << _request->get_method() << " is not available in this Worker service." RESET << std::endl;
    return false;
}

bool Worker::is_valid_method()
{
    for (std::vector<std::string>::iterator it = _config.routes[_route].accepted_methods.begin(); it != _config.routes[_route].accepted_methods.end(); ++it) {
        if (*it == _request->get_method()) {
            std::cout << GREEN "Method " << _request->get_method() << " is valid for this route." RESET << std::endl;
            return true;
        }
    }
    std::cout << RED "Method " << _request->get_method() << " is not valid for this Route." RESET << std::endl;
    return false;
}

bool Worker::servername_is_valid()
{
    if (_config.server_name == _request->get_header_element("Host")) {
        std::cout << GREEN "Server name in Header corresponds to this server." RESET << std::endl;
        return true;
    }
    std::cout << RED "Server name in Header does not correspond to this server." RESET << std::endl;   
    return false;
}
/*
bool Worker::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}
*/

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
    std::cout << GREEN "Worker service started verifications." RESET << std::endl;
    _status_code = 0;
    _use_cgi = false;
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
        check_cgi();
    }
    else
        _status_code = 404;    
    check_for_errors();
}

void Worker::clean_up()
{
    for (std::map<std::string, IHttpMethod*>::iterator it = _method_handlers.begin(); it != _method_handlers.end(); ++it) {
        delete it->second;
    }
    delete _request;
    std::cout << ORANGE "Worker service cleaned up." RESET << std::endl;    
}

Worker::~Worker()
{
    clean_up();
    std::cout << GREEN "Worker service ended cleanly." RESET << std::endl;
}


Response Worker::run()
{
    std::cout << GREEN "Worker service started working." RESET << std::endl;
    Response response;

    if (_status_code > 399) {
        response = Response(_status_code, "Error", _config);
        std::cout << RED "Error reponse generated from Worker: " << _status_code << RESET << std::endl;
        return response;
    }
    if (_status_code > 299) {
        response = Response(_status_code, "Redirect", _config);
        response.set_header("Location", _config.routes[_route].redirection_path);
        std::cout << GREEN "Redirect reponse generated from Worker: " << _status_code << RESET << std::endl;
        return response;
    }    
    if (_use_cgi) {        
        std::cout << ORANGE "Worker will execute CGI ." RESET << std::endl;

        response = execute_cgi();

        std::cout << GREEN "Response generated by CGI and returned." RESET << std::endl;
        return response;        
    }
    std::cout << ORANGE "Worker will call Handler for Method: " << _request->get_method() << " with " << _fullpath << RESET << std::endl;
    response = _method_handlers[_request->get_method()]->handle(*_request, _fullpath, _config, _route);
    std::cout << GREEN "Response generated by Method and returned." RESET << std::endl;
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

std::vector<std::string> Worker::build_cgi_environment()
{
    std::vector<std::string> envp;
    
    std::cout << ORANGE "Building CGI environment variables." RESET << std::endl;
    
    // Add static environment variables directly to envp
    envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envp.push_back("SERVER_SOFTWARE=Webserv/1.0");

    // Add environment variables from request to envp
    envp.push_back("PATH_INFO=" + _cgi_path); 
    envp.push_back("REQUEST_METHOD=" + _request->get_method());    
    envp.push_back("QUERY_STRING=" + _querystring);
    
    // Add various headers as environment variables if they exist
    if (_request->get_header_element("Content-Length") != "") {
        envp.push_back("CONTENT_LENGTH=" + _request->get_header_element("Content-Length"));
    }
    if (_request->get_header_element("Content-Type") != "") {
        envp.push_back("CONTENT_TYPE=" + _request->get_header_element("Content-Type"));
    }
    if (_request->get_header_element("Host") != "") {
        envp.push_back("HTTP_HOST=" + _request->get_header_element("Host"));
    }
    // Transform the port number to a string
    std::ostringstream oss;
    oss << _config.port;
    std::string port = oss.str();

    // Add path-related environment variables
    envp.push_back("SCRIPT_NAME=" + _fullpath);

    // Add server-related environment variables    
    envp.push_back("SERVER_NAME=" + _config.server_name);
    envp.push_back("SERVER_PORT=" + port);
    
    
    std::cout << YELLOW "Environment variables for CGI-Script: " RESET << std::endl;
    for (std::vector<std::string>::iterator it = envp.begin(); it != envp.end(); ++it) {
        std::cout << YELLOW << *it << RESET << std::endl;
         }

    return envp;
}

Response Worker::execute_cgi() {    
    // Create pipe for communication between parent and child process
    int pipefd[2];
    pipe(pipefd);

    // Create a response to store the result of the CGI script
    Response result;
    std::string resultstring;

    // Fork the process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute CGI script
        std::cerr << BLUE "Child process created that will run the CGI script: " << _fullpath << RESET << std::endl;
        
        // Build the environment for execve
        std::vector<std::string> envp_string = build_cgi_environment();
        
        // Convert the environment to a char* array
        char *envp[envp_string.size() + 1];
        for (size_t i = 0; i < envp_string.size(); i++) {
            envp[i] = new char[envp_string[i].size() + 1];
            std::strcpy(envp[i], envp_string[i].c_str());
        }
        envp[envp_string.size()] = NULL;
        envp_string.clear();

        // Redirect stdin and stdout to pipe        
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);               // Close unused read end
        char *argv[2];
        // Execute the script with the custom environment and arguments

        // Find the correct CGI script to execute and prepare argv
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {
            if (it->get_name() == _cgi_type) {
                argv[0] = new char[it->get_path().size() + 1];
                std::strcpy(argv[0], it->get_path().c_str());
                argv[1] = NULL;
            }
        }
        std::cerr << BLUE "Starting of CGI script immediate: " << _fullpath << RESET << std::endl;
        if (execve(argv[0], argv, envp) == -1) {
            std::cerr << BLUE "Starting of CGI script failed: " << _fullpath << ". Clean up." << RESET << std::endl;
            // Clean up
            int i = 0;
            while (envp[i] != NULL) {
                //std::cout << "Deleting envp: " << envp[i] << std::endl;
                delete [] envp[i];
                i++;
                }
            delete[] argv[0];
            argv[0] = new char[14];
            std::strcpy(argv[0], "./destructor");

           // _config.tab_cgi.clear();            
           // _config.routes.clear();
           // _config.tab_cgi.clear();
            //std::cout << "Starting of CGI script failed. Clean up." << std::endl;
            // 
           execve(argv[0], argv, NULL);            

            clean_up();
            exit(1);
        //check for necessary cleanup
                    
        }
    }
    else {
        // Parent process: Read script output from pipe
        std::cout << ORANGE "Continuation of parent process waiting for result of CGI SCript in Child process: " << _fullpath << RESET << std::endl;

        // Close unused write end of pipe
        close(pipefd[1]);

        // Read from pipe and append to result string
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            resultstring.append(buffer, bytes_read);
           // std::cout << "Read " << bytes_read << " bytes: " << buffer << std::endl;
        }
        std::cout << ORANGE "Result of CGI script: " RESET << std::endl << resultstring << std::endl;

        // Close read end of pipe
        close(pipefd[0]);

        // Wait for child process to finish
        int status;
        int exit_code;
        time_t start_time = time(NULL);
        while (1)
        {
            time_t elapsed_time = time(NULL) - start_time;
            if (elapsed_time > _cgi_timeout) {
                std::cout << ORANGE "Timeout of CGI script reached." RESET << std::endl;
                exit_code = 417;
                kill(pid, SIGKILL);                
                break;
            }
    
        waitpid(pid, &status, WNOHANG);
        
        
        // Check if the child exited normally
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
            std::cout << ORANGE "Child process exited with code: " RESET << exit_code << std::endl;
            break;
        }
        // Check if the child was terminated by a signal
        if (WIFSIGNALED(status)) {
            exit_code = WTERMSIG(status);
            std::cout << ORANGE "Child process was terminated with signal: " RESET << exit_code << std::endl;
            break;
        }
        std::cout << ORANGE "Waiting for child process to finish." RESET << std::endl;
        //  sleep(1);
        }
        if (exit_code == 418) {
            std::cout << ORANGE "Child process exited with error code: " RESET << exit_code << std::endl;
            result = Response(500, "Internal Server Error", _config);            
        }
        else if (exit_code == 417) {
            std::cout << ORANGE "Child process timed out." RESET << std::endl;
            result = Response(500, "Internal Server Error", _config);            
        }
        else {
            std::cout << ORANGE "CGI Script exited normally." RESET << std::endl;
            result = Response(resultstring);
        }
        


    }
    return result;
}