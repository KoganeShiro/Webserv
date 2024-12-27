#include "WebServ.hpp"

//----AJOUT DAMIEN----
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 1234
#define BUFFER_SIZE 4096
#define MAX_EVENTS 10
//----FIN AJOUT DAMIEN----


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



//---- AJOUT DAMIEN ----

//For inserting a file content into a buffer
    std::string loadFileContent(const std::string& filePath) 
    {
        std::ifstream file(filePath.c_str(), std::ios::in);
        if (!file) 
        {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return "";
        }

        std::string content;
        std::string line;

        while (std::getline(file, line)) 
        {
            content += line; // GNL
        }
        file.close();
        return (content);
    }

void    run_server(void)
{
    int fdSocket = socket(AF_INET, SOCK_STREAM, 0); //create_spcket()->Socket.cpp
    if (fdSocket < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // --bind_socket()->Socket.cpp--
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt = 1;
	if (setsockopt(fdSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
	    perror("setsockopt failed");
	    exit(EXIT_FAILURE);
	}

    if (bind(fdSocket, (const sockaddr *)(&address), sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocket, 10) < 0) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) 
    {
        perror("Epoll create failed");
        exit(EXIT_FAILURE);
    }

    epoll_event event;
    event.data.fd = fdSocket;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fdSocket, &event);

    epoll_event events_tab[MAX_EVENTS];

    --
    server_run();
}

void    prepare_server_to_run(void)
{

}
//----FIN AJOUT DAMIEN---

int main(int argc, char **argv)
{


    if (argc != 2) {
        std::cout <<
            RED "Usage: ./WebServ <configuration file>" RESET
        << std::endl;
        return ;
    }
    // setup_server(argv[1]); //configuration parsing
        //Server server(setup_server(argv[1]));

    //AJOUT DAMIEN
        //Socket socket(server._data.port);

    // listen_on_socket(); //will call request_parser()
    // worker_response(); //send the response of the request
    
// Load configuration
    const std::string config = argv[1];
    Server server(config);
// Initialize server
    server.run();
    
    return (0);
}