#include "./../includes/WebServ.hpp"

#include <fstream>
#include <string>
#include <sstream>

#define PORT 1234
#define BUFFER_SIZE 4096
#define INDEX_PATH "./../html_page/www/html/index.html"


std::string readFileIntoString(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
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
    Request *request;
  while (active)
  {
    unsigned long resultLen = sizeof(sockaddr);
    std::cout << "Listening on Port: " << PORT << std::endl;
    connection = accept(fdSocket, (struct sockaddr *)(&address), (socklen_t *)&resultLen);
    char buffer[BUFFER_SIZE];

  while (1) {
    ssize_t bytesRead = read(connection, buffer, BUFFER_SIZE);
    request->add_to_request(buffer); //??
    if (request->get_is_ready() == true) break;
    else continue;
   }

    Request *new_request = request->parsed_request();
  
    std::cout << "Request:" << std::endl;
    std::cout << buffer << std::endl;

    std::string content = readFileIntoString(INDEX_PATH);
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;
    send(connection, response.c_str(), response.size(), 0);
    close(connection);
  }

  close(fdSocket);
  delete request;

  return (EXIT_SUCCESS);
}

int main()
{
  server();
  return 0;
}
