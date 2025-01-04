
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


void set_cgi_environment(const std::map<std::string, std::string>& headers, const std::string& method, const std::string& query_string) {
    setenv("REQUEST_METHOD", method.c_str(), 1);
    setenv("QUERY_STRING", query_string.c_str(), 1);
    if (headers.find("Content-Length") != headers.end()) {
        setenv("CONTENT_LENGTH", headers.at("Content-Length").c_str(), 1);
    }
    if (headers.find("Content-Type") != headers.end()) {
        setenv("CONTENT_TYPE", headers.at("Content-Type").c_str(), 1);
    }
    setenv("SCRIPT_NAME", "/cgi-bin/hello", 1); // Example script
    setenv("SERVER_NAME", "localhost", 1);
    setenv("SERVER_PORT", "8080", 1);
}

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

void execute_cgi(const std::string& script_path) {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute CGI script
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[0]);              // Close unused read end
        execl(script_path.c_str(), script_path.c_str(), nullptr);
        perror("execl failed");        // If execl fails
        exit(1);
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
*/