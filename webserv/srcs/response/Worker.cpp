
//#include "WebServ.hpp"

#include "Worker.hpp"

void Worker::check_cgi()
{
    std::cout << YELLOW "Check if CGI is available and parse information from Path." RESET << std::endl;
    std::string cgitype;
    if (_config.routes[_route].use_cgi)
    {
        size_t start_query_str = _fullpath.find('?');
        if (start_query_str != std::string::npos)
        {
            _querystring = _fullpath.substr(start_query_str + 1);
            _fullpath = _fullpath.substr(0,start_query_str);
        }
        //std::string extension_path_query = _fullpath.substr(_fullpath.find('.') + 1);
        //std::string extension_path;
        std::string cgitype;
     
        size_t extension_start_pos = _fullpath.find('.');
        std::cout << BLUE "Extension start pos: " RESET << extension_start_pos << std::endl;
        size_t extension_path_start_pos = _fullpath.find('/', extension_start_pos);
        std::cout << BLUE "Extension path start pos: " RESET << extension_path_start_pos << std::endl;
        if (extension_path_start_pos == std::string::npos) {
            extension_path_start_pos = _fullpath.length();
        }
        std::cout << BLUE "Extension path start pos: " RESET << extension_path_start_pos << std::endl;
        _cgi_path = _fullpath.substr(extension_path_start_pos);
        std::cout << BLUE "CGI Path: " RESET << _cgi_path << std::endl;
        
        _fullpath = _fullpath.substr(0, extension_path_start_pos);
        std::cout << BLUE "Fullpath: " RESET << _fullpath << std::endl;
        if (extension_start_pos != std::string::npos) {            
            cgitype = _fullpath.substr(extension_start_pos + 1, extension_path_start_pos - extension_start_pos - 1);
        }
        std::cout << BLUE "CGI Type: " RESET << cgitype << std::endl;
        
        

//            std::string extension_path_query = _fullpath.substr(extension_path_pos + 1);
//            _cgi_path = extension_path_query;
            
        //    extension_path_query = extension_path_query.substr(0, extension_path_query.find('?'));
 //       }
 //       if (extension_path_query.find('/') != std::string::npos) {
            // Check the meaning of the line in the subject: 
            // Because you won’t call the CGI directly, use the full path as PATH_INFO.
            // maybe it means that the Path_Info is the full path to the CGI script
            
   //         _cgi_path = extension_path_query.substr(extension_path_query.find('/'));            
     //       extension_path_query = extension_path_query.substr(0, extension_path_query.find('/'));        
       //     _fullpath = _fullpath.substr(0, _fullpath.find('.') + 1) + extension_path_query;
            // alternative
            // _cgi_path = _fullpath;
            

   //     }                
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {   
            std::cout << YELLOW "Checking CGI type: " RESET << it->get_name() << std::endl;         
            if (it->get_extension() == cgitype) {
           // if (it->get_extension() == extension_path_query) {
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
    if (_config.server_name == _request->get_header_element("Host").substr(0, _request->get_header_element("Host").find(':'))) {
        std::cout << GREEN "Server name in Header corresponds to this server." RESET << std::endl;
        return true;
    }
    else if (_config.server_name == "") {
        std::cout << GREEN "No server name in configuration, accepting Server name in Header." RESET << std::endl;
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
        _error_message = "Method Not Allowed";
        std::cout << "Method not allowed" << std::endl;
        return;
    }
    if (! servername_is_valid()) {
        _status_code = 404;
        _error_message = "Server Not Found";
        std::cout << "Server name not found" << std::endl;
        return;
    }
    if (_route.empty()) {
        _status_code = 404;
        _error_message = "Route Not Found";
        std::cout << "Route not found" << std::endl;
        return;
    }
    if (_config.routes[_route].redirection_nb > 299 && _config.routes[_route].redirection_path != "") {
        _status_code = _config.routes[_route].redirection_nb;
        _error_message = "Redirect";
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
        if (_route == "/")
            _fullpath = _config.routes[_route].root_dir + _request->get_path();
        else
            _fullpath = _config.routes[_route].root_dir + (_request->get_path().substr(_route.length()));
        std::cout << ORANGE "Fullpath check: " RESET << _fullpath << std::endl;
        
        if (_fullpath[_fullpath.length() - 1] == '/')            
           _fullpath += _config.routes[_route].default_file;
        check_cgi();
        std::cout << ORANGE "Fullpath check: " RESET << _fullpath << std::endl;
    }
    else {
        _status_code = 404;
        _error_message = "Route Not Found";
    }
    std::cout << ORANGE "Worker service finished verifications. Fullpath : " RESET << _fullpath <<  std::endl;
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
        response = Response(_status_code, _error_message, _config);
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
        std::cout << ORANGE "Worker will execute CGI." RESET << std::endl;
        response = execute_cgi();
        std::cout << GREEN "Response generated by CGI and returned." RESET << std::endl;
        std::cout << response.http_response() << std::endl;
        return response;        
    }
    std::cout << ORANGE "Worker will call Handler for Method: " << _request->get_method() << " with " << _fullpath << RESET << std::endl;
    response = _method_handlers[_request->get_method()]->handle(*_request, _fullpath, _config, _route);
    std::cout << GREEN "Response generated by Method and returned:" RESET << std::endl;
    std::cout << response.http_response() << std::endl;
    return response;    
}


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
    // if this is set as passinfo the cgi tester hangs
    //envp.push_back("PATH_INFO=" + _fullpath);
    envp.push_back("PATH_TRANSLATED=" + _fullpath);
    
    //envp.push_back("PATH_INFO=" + _request->get_path().substr(0, _request->get_path().find('?')));

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
    // envp.push_back("SCRIPT_NAME=" + _request->get_path().substr(0, _request->get_path().find('?'))); 
    envp.push_back("REQUEST_URI=" + _fullpath);


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
    int reqpipe[2];
    pipe(pipefd);
    pipe(reqpipe);

    // Create a response to store the result of the CGI script
    Response result;
    std::string resultstring;

    // Fork the process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute CGI script
        std::cerr << BLUE "--- Child process created that will run the CGI script: " << _fullpath << RESET << std::endl;
        
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


        char *argv[3];
        // Execute the script with the custom environment and arguments

        // Find the correct CGI script to execute and prepare argv
        for (std::vector<CGI>::iterator it = _config.tab_cgi.begin(); it != _config.tab_cgi.end(); ++it) {
            if (it->get_name() == _cgi_type) {
                argv[0] = new char[it->get_path().size() + 1];
                std::strcpy(argv[0], it->get_path().c_str());
                argv[1] = new char[_fullpath.size() + 1];
                std::strcpy(argv[1], _fullpath.c_str());
                argv[2] = NULL;
                std::cerr << BLUE "--- CGI script runtime: " << argv[0] << RESET << std::endl;
            }
        }
        std::cerr << BLUE "--- Starting of CGI script immediate: " << _fullpath << RESET << std::endl;
        
        // Redirect stdin and stdout to pipe        
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);               // Close unused read end

        dup2(reqpipe[0], STDIN_FILENO); // Redirect stdin to pipe
        close(reqpipe[1]);                       // Close unused write end
        close(reqpipe[0]);
        // Change working directory to the directory of the CGI script
        chdir(_fullpath.substr(0, _fullpath.find_last_of('/')).c_str());

        if (execve(argv[0], argv, envp) == -1) {
            std::cerr << BLUE "--- Starting of CGI script failed: " << _fullpath << ". Clean up." << RESET << std::endl;
            // Clean up
            int i = 0;
            while (envp[i] != NULL) {
            //std::cout << "Deleting envp: " << envp[i] << std::endl;
                delete [] envp[i];
                i++;
                }
            delete[] argv[0];
            delete[] argv[1];
            std::cerr << RED "--- Execute destructor instead of cleaning up manually." RESET << std::endl;

            // Execute destructor instead of cleaning up manually
            argv[0] = new char[14];
            std::strcpy(argv[0], "./destructor");
            execve(argv[0], argv, NULL);

            // Test to clean up manually that was not conclusive

           // _config.tab_cgi.clear();            
           // _config.routes.clear();
           // _config.tab_cgi.clear();
            //std::cout << "Starting of CGI script failed. Clean up." << std::endl;
            // 
                  

            clean_up();
            exit(1);
        //check for necessary cleanup
                    
        }
    }
    else {
        // Parent process: Read script output from pipe
        std::cout << ORANGE "Continuation of parent process waiting for result of CGI script in child process: " << _fullpath << RESET << std::endl;

        // Close unused write end of pipe
        close(pipefd[1]);

        // Close unused read end of pipe
        close(reqpipe[0]);

        // Write request body to requestpipe
        std::string body = _request->get_body();
        write(reqpipe[1], body.c_str(), body.length());
     //   std::cout << ORANGE "Request body sent to CGI script." RESET << body << std::endl;
      //  write(2, body.c_str(), body.length());
        close(reqpipe[1]);

        

        
        // Set up timeout for CGI script
        time_t start_time = time(NULL);        
        int exit_code;



        // Read from pipe and append to result string
        char buffer[1024];
        ssize_t bytes_read;

        // Set up select for timeout
        fd_set read_fds;
        struct timeval timeout;
        std::cout << ORANGE "Waiting for child process to finish." RESET << std::endl;
        while (true) {
            // Set up file descriptor set for select
            FD_ZERO(&read_fds);
            FD_SET(pipefd[0], &read_fds);

            // Calculate remaining time
            time_t elapsed_time = time(NULL) - start_time;
            // If the timeout 
            timeout.tv_sec = _cgi_timeout - elapsed_time;
            timeout.tv_usec = 0;

            // Check if the timeout has been reached
            int ret = select(pipefd[0] + 1, &read_fds, NULL, NULL, &timeout);

            // If the select call returns 0, the timeout has been reached
            if (ret == 0) {
                std::cout << RED "Timeout of CGI script reached." RESET << std::endl;                
                kill(pid, SIGKILL);
                break;
            }
            else {
                // Read from pipe and append to result string
                if (FD_ISSET(pipefd[0], &read_fds)) {
                    bytes_read = read(pipefd[0], buffer, sizeof(buffer));
                    if (bytes_read > 0) {
                        resultstring.append(buffer, bytes_read);
                    }
                else {
                    break;
                }
                }
            }
        }       
        // Close read end of pipe
        close(pipefd[0]);
        close(reqpipe[1]);



        // Wait for child process to finish
        int status;
        waitpid(pid, &status, 0);
        // Check if the child exited normally
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
            std::cout << ORANGE "Child process exited with code: " RESET << exit_code << std::endl;
            
        }
        // Check if the child was terminated by a signal
        if (WIFSIGNALED(status)) {
            exit_code = WTERMSIG(status);
            std::cout << ORANGE "Child process was terminated with signal: " RESET << exit_code << std::endl;            
        }        
        std::cout << ORANGE "Result of CGI script: " RESET << std::endl << resultstring << std::endl;                
        if (exit_code == 9) {
            std::cout << ORANGE "Child process timed out." RESET << std::endl;
            result = Response(500, "Internal Server Error", _config);            
        }
        else if (exit_code == 0) {
            std::cout << ORANGE "CGI Script exited normally." RESET << std::endl;
            result = Response(resultstring);
        }
        else if (exit_code == 418) {
            std::cout << ORANGE "Child process exited with error code: " RESET << exit_code << std::endl;
            result = Response(500, "Internal Server Error", _config);            
        }
        else {
            std::cout << ORANGE "Child process exited with error code: " RESET << exit_code << std::endl;
            result = Response(500, "Internal Server Error", _config);            
        }
    }
    return result;
}