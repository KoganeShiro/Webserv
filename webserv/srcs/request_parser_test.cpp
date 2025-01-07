#include "WebServ.hpp"
#include "Request.hpp"

#define PORT 1234
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
		std::cout << "Request Buffer:\n`" << request->get_request_buffer() << "`" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << "Method: `" << request->get_method() << "`" << std::endl;
		std::cout << "Path: `" << request->get_path() << "`" << std::endl;
		std::cout << "HTTP Version: `" << request->get_http_version() << "`" << std::endl;
		std::cout << "Content Length: `" << request->get_content_length() << "`" << std::endl;
		std::cout << "Good Request: `" << (request->get_good_request() ? "Yes" : "No") << "`" << std::endl;
		std::cout << "Is Ready: `" << (request->get_is_ready()) << "`" << std::endl;

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

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 90
#endif

void read_request()
{
    std::string request =
        "POST /submit-form HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
		//"Content-Length: 20\r\n"
		"Transfer-Encoding: chunked\r\n"
        "\r\n"
        //"name=John+Doe&age=30&city=NY";
		// "6\r\n"
		// "Hello,\r\n"
		// "6\r\n"
		// "world!\r\n"
		// "0\r\n"
		// "\r\n"
		"27\r\n"
		"Voici les données du premier morceau\r\n\r\n"
		"1C\r\n"
		"et voici un second morceau\r\n\r\n"
		"20\r\n"
		"et voici deux derniers morceaux \r\n"
		"12\r\n"
		"sans saut de ligne\r\n"
		"0\r\n"
		"\r\n"
		"GET / HTTP/1.1\r\n";

    std::istringstream stream(request);
    char buffer[BUFFER_SIZE];
    Request req;
    ssize_t bytesRead = 1;
    int max_iterations = 100; // Set a reasonable maximum iteration limit

    for (int i = 0; i < max_iterations; ++i) {
        stream.read(buffer, BUFFER_SIZE - 1);
        bytesRead = stream.gcount();
        if (bytesRead <= 0) {
            break;
        }
        buffer[bytesRead] = '\0';
        std::cout << ORANGE << buffer << RESET << std::endl;
        req.add_to_request(buffer);
        if (req.get_is_ready() != AGAIN) {
            break;
        }
        std::cout << YELLOW "one loop" RESET << std::endl;
    }

    if (req.get_is_ready() == GOOD) {
        Request* new_request = req.parsed_request();

        std::cout << GREEN "Request (new_request):" RESET << std::endl;
        print_Request(new_request);
		delete new_request;
    } else {
        std::cout << CYAN "Request BAD HEADER:" RESET << std::endl;
        print_Request(&req);
    }
	std::cout <<
		"req remainder: [" << req.get_request_buffer() << "]"
	<< std::endl; 
}

// std::string readFileIntoString(const std::string& filename)
// {
//     std::ifstream file(filename.c_str());
//     if (!file.is_open()) {
//         return "<h1>ERROR opening index.html</h1>";
//     }
		
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     file.close();
		
//     return (buffer.str());
// }

// int server()
// {

//   int fdSocket = socket(AF_INET, SOCK_STREAM, 0);

//   sockaddr_in address;
//   address.sin_family = AF_INET;
//   address.sin_port = htons(PORT);
//   address.sin_addr.s_addr = htonl(INADDR_ANY);

//   bind(fdSocket, (const sockaddr *)(&address), sizeof(address));

//   listen(fdSocket, 10);

//   bool active = true;
//   int connection;
//     Request *new_request;
//     Request request;
//     ssize_t bytesRead;
//   while (active)
//   {
//     unsigned long resultLen = sizeof(sockaddr);
//     std::cout << "Listening on Port: " << PORT << std::endl;
//     connection = accept(fdSocket, (struct sockaddr *)(&address), (socklen_t *)&resultLen);
//     char buffer[BUFFER_SIZE];

//   while (1) {
//     bytesRead = read(connection, buffer, BUFFER_SIZE - 1);
//     request.add_to_request(buffer); //??
//     if (request.get_is_ready() == true) break;
//     else continue;
//    }
//     new_request = request.parsed_request();
	
//     std::cout << "Request:" << std::endl;
//     std::cout << buffer << std::endl << "==================" << std::endl;

//     print_Request(new_request);

//     std::string content = readFileIntoString(INDEX_PATH);
//     std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + to_string(content.length()) + "\n\n" + content;
//     send(connection, response.c_str(), response.size(), 0);
//     close(connection);
//   }

//   close(fdSocket);
//   delete (new_request);

//   return (EXIT_SUCCESS);
// }

int main()
{
	//server();
	read_request();
	return 0;
}
