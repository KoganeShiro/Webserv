#include "Response.hpp"
#include "Server.hpp"
#include "Worker.hpp"

struct ConnectionInfo {
    Connection* connection;
    Config_data data;
};
/*
Request *get_data_from_connection(ConnectionInfo client_connection)
{
    int client_fd = client_connection.connection->get_clientfd();    
    char buffer[1024];
    std::string data;
    ssize_t bytes_received;
    Request* request = client_connection.connection->get_request();
    int answer;

    while (1) {
        bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        data.append(buffer, bytes_received);
        //std::cout << MAGENTA << data << RESET << std::endl;
        answer = request->add_to_request(data, client_connection.data.client_body_size_limit);
        if (answer == BAD_HEADER || answer == GOOD)
            break;
        // else if (answer == MULTIPART_FORM_DATA) {
        // }
        data.erase();
    }

    if (bytes_received < 0) {
        std::cerr << "Error when reading client's socket : " << client_fd << std::endl;
        return (NULL);
    }
    return (request);
}
*/

int get_data_from_connection(ConnectionInfo client_connection)
{
    int client_fd = client_connection.connection->get_clientfd();    
    char buffer[100];
    std::string data;
    ssize_t bytes_received;
    Request* request = client_connection.connection->get_request();
    int answer;

 //   while (1) {
        bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0) {
            return (BAD_HEADER); // check if another response would be better
     //       break;
        }
        buffer[bytes_received] = '\0';
        data.append(buffer, bytes_received);
        //std::cout << MAGENTA << data << RESET << std::endl;
        answer = request->add_to_request(data, client_connection.data.client_body_size_limit);
        //if (answer == BAD_HEADER || answer == GOOD)
        //    break;
        // else if (answer == MULTIPART_FORM_DATA) {
        // }
        data.erase();

   // }
    if (bytes_received < 0) {
        std::cerr << "Error when reading client's socket : " << client_fd << std::endl;
        return (BAD_HEADER); // check if another response would be better
    }
    return (answer);
}


ConnectionInfo find_connection(int client_fd, std::vector<Server*> servers)
{
    ConnectionInfo ci;
    ci.connection = NULL;
// est-ce que il faut vraiment parcourir tous les serveurs ici?
    for (size_t i = 0 ; i < servers.size() ; ++i) {
        if (servers[i]->get_socket_fd() == client_fd) {
            ci.connection = servers[i]->add_connection(); // removed by Georg
            Config_data data = servers[i]->get_data();
            ci.data = data;
            std::cout << "End of find_connection\n";
            return (ci);
        }
        ci.connection = servers[i]->get_connection_by_fd(client_fd);
        if (ci.connection != NULL) {
            ci.data= servers[i]->get_data();
            return (ci);
        }
    }
    return (ci);
}

void    ft_manage_answer(Request* request, ConnectionInfo connection)
{
    int answ = request->get_is_ready();
    //std::cout << "answer :" << answ << std::endl;
    if (answ == BAD_HEADER) {
        //send(error 400) 
        Response response(400, "Bad Request", connection.data);
        std::string str = response.http_response(); //call generate_error_page
        std::cout << RED << str << RESET << std::endl;
        ssize_t bytes_sent = send(connection.connection->get_clientfd(), str.c_str(), str.length(),0);
        if (bytes_sent == -1) {
         //   perror("write");
            std::cerr << "Error when sending response to client" << std::endl;
        }
    }
    else if (answ == AGAIN) { // Don't understand why Georg
        connection.connection->set_request_is_done(0); 
    }
    else if (answ == GOOD) {
        connection.connection->set_request_is_done(1);
        Request* new_request = request->parsed_request();
        Worker  bob(connection.data, new_request);
        Response response = bob.run();
        std::string str = response.http_response(); //call generate_error_page
        //std::cout << GREEN << str << RESET << std::endl;
        ssize_t bytes_sent = send(connection.connection->get_clientfd(), str.c_str(), str.length(),0);
        if (bytes_sent == -1) {
           // perror("write");
           std::cerr << "Error when sending response to client" << std::endl;
        }
    }
}

void    print_listen(std::vector<Server*> servers)
{
    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "Listening to " BLUE;
        if (servers[i]->get_data().server_name.empty()) {
            std::cout << servers[i]->get_data().host;
        }
        else {
            std::cout << servers[i]->get_data().server_name;
        }
        std::cout << ":" << servers[i]->get_data().port << RESET
        << std::endl;
    }
}
int is_server_socket(int fd, std::vector<Server*> servers)
{
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i]->get_socket_fd() == fd) {
            return (1);
        }
    }
    return (0);
}


void add_client_to_epoll(int epoll_fd, int client_fd)
{
    epoll_event event;
    event.data.fd = client_fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) < 0) {
        std::cerr << "Failed to add client to epoll: " << strerror(errno) << std::endl;
        // throw std::runtime_error("Failed to add client to epoll");
    }
    std::cout << MAGENTA "Clientsocket added to epoll: " RESET << client_fd << std::endl;
}


void remove_client_from_epoll(int epoll_fd, int client_fd)
{
  //  epoll_event event;
   // event.data.fd = client_fd;
   // event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) < 0) {
        std::cerr << RED "Failed to remove client from epoll: " RESET << strerror(errno) << std::endl;
        // throw std::runtime_error("Failed to remove client from epoll");
    }
    std::cout << MAGENTA "Clientsocket removed from epoll: " RESET << client_fd << std::endl;    
}


void    run_epoll(int epoll_fd, std::vector<Server*> servers)
{
    epoll_event events[MAX_EVENTS];
    // vector de connections 
    std::map<int, ConnectionInfo> connections;

    std::cout << GREEN << "WAITING FOR THE FIRST REQUEST...\n" << RESET;
    while (true) {
        print_listen(servers);

        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        
        if (event_count < 0) {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            int socket = events[i].data.fd;
            if (events[i].events & EPOLLIN) {
                //int client_fd = events[i].data.fd;
                 usleep(500000); // helps to see the debug log
                if (is_server_socket(socket, servers)) {
                    std::cout << MAGENTA "Event at (Server-)socket: " RESET << socket << std::endl;

                    ConnectionInfo client_connection = find_connection(socket, servers);
                    if (client_connection.connection == NULL) {
                        std::cerr << RED "Error when adding connection" RESET << std::endl;
                        continue;
                    }
                    add_client_to_epoll(epoll_fd, client_connection.connection->get_clientfd());
                    connections[client_connection.connection->get_clientfd()] = client_connection;
                    std::cout << MAGENTA "New client connected at : fd = " RESET << client_connection.connection->get_clientfd() << std::endl;
                }
                else {
                    std::cout << MAGENTA "Event at Client-socket: " RESET << socket << std::endl;
                    int answer;
                    if (connections.find(socket) == connections.end()) {
                        std::cerr << RED "Error when finding connection" RESET << std::endl;
                        remove_client_from_epoll(epoll_fd, socket);
                        continue;
                    }
                    ConnectionInfo client_connection = connections[socket];
                    if (client_connection.connection == NULL) {
                        std::cerr << RED "Error when accessing connection" RESET << std::endl;
                        continue;
                    }
                    answer = get_data_from_connection(client_connection);
                    std:: cout << MAGENTA "Request parser status for client " RESET << socket << MAGENTA " is : " RESET << answer << std::endl;
                    if (answer == BAD_HEADER || answer == GOOD) {                        
                        ft_manage_answer(client_connection.connection->get_request(), client_connection);
                        std::cout << MAGENTA "Request processed (Response sent) for client: " RESET << socket << std::endl;
                        
                        remove_client_from_epoll(epoll_fd, socket);
                        client_connection.connection->close();
                        for (size_t i = 0; i < servers.size(); i++) {
                            servers[i]->remove_connection(socket);
                        }
                        delete client_connection.connection;
                        connections.erase(socket);
                        std::cout << MAGENTA "Client connection closed and removed from epoll: " RESET << socket << std::endl;
                    }
                    else if (answer == AGAIN) {
                        std:: cout << YELLOW "Request for client " RESET << socket << YELLOW "received partially. Will continue parsing next time." RESET << std::endl;
                        continue;
                    }
               //     else {
                 //       client_connection.connection->close();
                  //      delete client_connection.connection;
                   // }
                }
            }
        }
        std::cout << GREEN << "WAITING FOR THE NEXT REQUEST...\n" << RESET;
    }
}