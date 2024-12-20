
#include "WebServ.hpp"

// void    setup_server(char *config_file)
// {
//     //open the file, check for errors and exit if necessary
//     //parse the file and fill struct/class
// }

// void    request_parser()
// {
//     //parse the header in param
//     //check for errors and exit if necessary (function)

//     //if nothing is wrong AND no body require
//         //worker_response()
//     //else ask socket to give body (and call worker_response() function)
// }

// void    listen_on_socket()
// {
//     //fill buffer until end of header (\r\n\r\n)
//     //put this buffer in request_parser() param
//     //on hold
//     request_parser();
//     //if request_parser no ERROR
//     // if require body, resume and fill the buffer
//     worker_response();
// }

// void    worker_response()
// {
//     //GET function
//         // check if path is good, send response
//         //may use CGI
//     //POST function
//         // create/modify the require data
//         //may use CGI
//     //DELETE function
//         // delete the require data
//         //may use CGI
//     //GET ERROR PAGE
//         //--> have a template where code error and text will be change dynamically ?
// }

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            RED "Usage: ./WebServ <configuration file>" RESET
        << std::endl;
    }
    // setup_server(argv[1]); //configuration parsing
    // listen_on_socket(); //will call request_parser()
    // worker_response(); //send the response of the request
    
// Load configuration
    const std::string config = argv[1];
    Server server(config);
// Initialize server
    server.run();
    
    return (0);
}