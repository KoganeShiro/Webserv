#include "Response.hpp"
#include "Server.hpp"
#include "Worker.hpp"

struct ConnectionInfo {
    Connection* connection;
    Config_data data;
};

Request *get_data_from_connection(ConnectionInfo client_connection) {
    int client_fd = client_connection.connection->get_clientfd();    
    char buffer[1024];
    std::string data;
    ssize_t bytes_received;
    Request* request = client_connection.connection->get_request();
    int answer;

    while (1) {
        bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0){
           break;
        }
        buffer[bytes_received] = '\0';
        data.append(buffer);
        std::cout << "data : " << data << std::endl;
        answer = request->add_to_request(data,client_connection.data.client_body_size_limit);
        std::cout << "ANSWER = " << answer << std::endl;
        if (answer == BAD_HEADER || answer == GOOD)
            break;
    }
    if (bytes_received < 0) {
        std::cerr << "Error when reading client's socket : " << client_fd << std::endl;
        return (NULL);
    }
    std::cout << "End of read\n";
    return (request);
}


ConnectionInfo find_connection(int client_fd, std::vector<Server*> servers){
    ConnectionInfo ci;
    ci.connection = NULL;
    for (size_t i = 0 ; i < servers.size() ; ++i){
        if (servers[i]->get_socket_fd() == client_fd){
            ci.connection = servers[i]->add_connection();
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

void    ft_manage_answer(Request* request, ConnectionInfo connection){
    int answ = request->get_is_ready();
    std::cout << "answer :" << answ << std::endl;
    if (answ == BAD_HEADER){
        //send(error 400) 
        Response response(400, "Bad Request", connection.data);
        std::string str = response.http_response(); //call generate_error_page
        std::cout << RED << str << RESET << std::endl;
        ssize_t bytes_sent = send(connection.connection->get_clientfd(), str.c_str(), str.length(),0);
        if (bytes_sent == -1) {
            perror("write");
        }
        //send(error 400) VOIR JORG
    }
    else if (answ == AGAIN){
        connection.connection->set_request_is_done(0);
    }
    else if (answ == GOOD){
        connection.connection->set_request_is_done(1);
        Request* new_request = request->parsed_request();
        Worker  bob(connection.data, new_request);
        Response response = bob.run();
        std::string str = response.http_response(); //call generate_error_page
        std::cout << GREEN << str << RESET << std::endl;
        ssize_t bytes_sent = send(connection.connection->get_clientfd(), str.c_str(), str.length(),0);
        if (bytes_sent == -1) {
            perror("write");
        }
    }
}

void    run_epoll(int epoll_fd, std::vector<Server*> servers)
{
    epoll_event events[MAX_EVENTS];

    std::cout << GREEN << "WAITING FOR THE FIRST REQUEST...\n" << RESET;
    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count < 0) {
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            if (events[i].events & EPOLLIN) {
                int client_fd = events[i].data.fd;
                ConnectionInfo client_connection = find_connection(client_fd, servers);
                Request* request = get_data_from_connection(client_connection);
                ft_manage_answer(request, client_connection);
                delete client_connection.connection;
            }
        }
    }
}