
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

Response handleCGI(const Request& request, const std::string& scriptPath
    // Execute CGI script
    // Capture output
    // Generate and return Response object
}
