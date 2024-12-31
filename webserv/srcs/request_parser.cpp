#include "WebServ.hpp"

#include <fstream>
#include <string>
#include <sstream>

#define PORT 1234
#define BUFFER_SIZE 4096
#define INDEX_PATH "./../html_page/www/html/index.html"

#include <iostream>

void print_Request(Request *request)
{
    if (request == NULL)
    {
        std::cout <<RED "Request is NULL" RESET << std::endl;
        return;
    }

    std::cout << "Request Details:" << std::endl;
    std::cout << "Request Buffer: `" << request->get_request_buffer() << "`" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Method: `" << request->get_method() << "`" << std::endl;
    std::cout << "Path: `" << request->get_path() << "`" << std::endl;
    std::cout << "HTTP Version: `" << request->get_http_version() << "`" << std::endl;
    std::cout << "Content Length: `" << request->get_content_length() << "`" << std::endl;
    std::cout << "Good Request: `" << (request->get_good_request() ? "Yes" : "No") << "`" << std::endl;
    std::cout << "Is Ready: `" << (request->get_is_ready() ? "Yes" : "No") << "`" << std::endl;

    std::cout << "\nHeaders:" << std::endl;
    std::cout << "--------" << std::endl;
    // Assuming you have a method to get all headers
    std::map<std::string, std::string> headers = request->get_header();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << "`" << it->first << "`" << ": "
         << "`" << it->second << "`" << std::endl;
    }

    std::cout << "\nBody:" << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << "`" << request->get_body() << "`" << std::endl;
}


std::string readFileIntoString(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return "<h1>ERROR opening index.html</h1>";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return (buffer.str());
}

int server()
{

  int fdSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(fdSocket, (const sockaddr *)(&address), sizeof(address));

  listen(fdSocket, 10);

  bool active = true;
  int connection;
    Request *new_request;
    Request request;
    ssize_t bytesRead;
  while (active)
  {
    unsigned long resultLen = sizeof(sockaddr);
    std::cout << "Listening on Port: " << PORT << std::endl;
    connection = accept(fdSocket, (struct sockaddr *)(&address), (socklen_t *)&resultLen);
    char buffer[BUFFER_SIZE];

  while (1) {
    bytesRead = read(connection, buffer, BUFFER_SIZE);
    request.add_to_request(buffer); //??
    if (request.get_is_ready() == true) break;
    else continue;
   }
    new_request = request.parsed_request();
  
    std::cout << "Request:" << std::endl;
    std::cout << buffer << std::endl << "==================" << std::endl;

    print_Request(new_request);

    std::string content = readFileIntoString(INDEX_PATH);
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + to_string(content.length()) + "\n\n" + content;
    send(connection, response.c_str(), response.size(), 0);
    close(connection);
  }

  close(fdSocket);
  delete (new_request);

  return (EXIT_SUCCESS);
}

int main()
{
  server();
  return 0;
}
