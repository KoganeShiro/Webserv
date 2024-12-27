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

std::string loadFileContent(const std::string& filePath) 
{
    std::ifstream file(filePath.c_str(), std::ios::in);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::string content, line;
    while (std::getline(file, line)) {
        content += line + "\n"; // Ajouter chaque ligne suivie d'un saut de ligne
    }
    file.close();
    return content;
}

int server_epoll() {
    int fdSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (fdSocket < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt = 1;
	if (setsockopt(fdSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
	    perror("setsockopt failed");
	    return EXIT_FAILURE;
	}

    if (bind(fdSocket, (const sockaddr *)(&address), sizeof(address)) < 0) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(fdSocket, 10) < 0) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("Epoll create failed");
        return EXIT_FAILURE;
    }

    epoll_event event;
    event.data.fd = fdSocket;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fdSocket, &event);

    epoll_event events[MAX_EVENTS];

    bool active = true;

    while (active) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n < 0) {
            perror("Epoll wait failed");
            break;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == fdSocket) {
                sockaddr_in clientAddr;
                socklen_t clientLen = sizeof(clientAddr);
                int clientSock = accept(fdSocket, (sockaddr *)&clientAddr, &clientLen);
                if (clientSock < 0) {
                    perror("Accept failed");
                    continue;
                }
        	
                std::cout << "Socker number : " << clientSock << " " << fdSocket << std::endl;
                epoll_event clientEvent;
                clientEvent.data.fd = clientSock;
                clientEvent.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSock, &clientEvent);
            } else if (events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead = read(events[i].data.fd, buffer, BUFFER_SIZE);
                if (bytesRead <= 0) {
                    close(events[i].data.fd);
                } else {
                    std::cout << "Received: " << buffer << std::endl;
			    std::string content = loadFileContent("./index.html");
                    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;
                    send(events[i].data.fd, response.c_str(), response.size(), 0);
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(fdSocket);
    close(epoll_fd);
    return EXIT_SUCCESS;
}

int main() {
    return server_epoll();
}

