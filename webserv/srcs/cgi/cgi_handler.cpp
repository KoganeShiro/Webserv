
#include "WebServ.hpp"

/*
When a CGI request is received, the server should:
    Set up environment variables (e.g., CONTENT_LENGTH)
    Fork a child process
    Connect the socket to standard input/output
    Execute the CGI script using execve()
    In the parent process:
        Wait for the child to complete
        Send the CGI script\'s output back to the client
*/

Response handleCGI(const Request& request, const std::string& scriptPath)
{
    // Execute CGI script
    // Capture output
    // Generate and return Response object
}
/*

Extract Query String (for GET):

For GET requests, extract the query string from the URL.
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

// Function to build the environment for execve
void build_cgi_environment(const std::map<std::string, std::string>& headers, 
                            const std::string& method, 
                            const std::string& query_string, 
                            std::vector<char*>& envp) {
    // Add environment variables directly to envp
    envp.push_back(const_cast<char*>(("REQUEST_METHOD=" + method).c_str()));
    envp.push_back(const_cast<char*>(("QUERY_STRING=" + query_string).c_str()));
    
    // Add headers as environment variables if they exist
    if (headers.find("Content-Length") != headers.end()) {
        envp.push_back(const_cast<char*>(("CONTENT_LENGTH=" + headers.at("Content-Length")).c_str()));
    }
    if (headers.find("Content-Type") != headers.end()) {
        envp.push_back(const_cast<char*>(("CONTENT_TYPE=" + headers.at("Content-Type")).c_str()));
    }

    // Add more static environment variables
    envp.push_back(const_cast<char*>("SCRIPT_NAME=/cgi-bin/hello"));  // Example script path
    envp.push_back(const_cast<char*>("SERVER_NAME=localhost"));
    envp.push_back(const_cast<char*>("SERVER_PORT=8080"));

    // Null-terminate the array (as required by execve)
    envp.push_back(nullptr);
}

void execute_cgi(const std::string& script_path, const std::map<std::string, std::string>& headers, 
                 const std::string& method, const std::string& query_string) {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute CGI script
        // Build the environment for execve
        std::vector<char*> envp;
        build_cgi_environment(headers, method, query_string, envp);

        // Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);               // Close unused read end

        // Prepare the argument list for execve (argv)
        const char *argv[] = { script_path.c_str(), nullptr };
        
        // Execute the script with the custom environment and arguments
        if (execve(script_path.c_str(), (char *const *)argv, envp.data()) == -1) {
            perror("execve failed");  // If execve fails
            exit(1);
        }
    } else {
        // Parent process: Read script output
        close(pipefd[1]); // Close unused write end
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            std::cout.write(buffer, bytes_read); // Send to client
        }
        close(pipefd[0]);
        waitpid(pid, nullptr, 0); // Wait for child process to finish
    }
}

int main() {
    // Example header map
    std::map<std::string, std::string> headers = {
        {"Content-Length", "1234"},
        {"Content-Type", "text/html"}
    };

    std::string method = "GET";
    std::string query_string = "id=123";

    // Execute CGI with the script path, method, query_string, and headers
    execute_cgi("/cgi-bin/hello", headers, method, query_string);

    return 0;
}
*/