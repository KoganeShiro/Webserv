#include "Response.hpp"
#include "Server.hpp"
#include "Worker.hpp"

struct ConnectionInfo {
    Connection* connection;
    Config_data data;
};

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


ConnectionInfo find_connection(int client_fd, std::vector<Server*> servers)
{
    ConnectionInfo ci;
    ci.connection = NULL;

    for (size_t i = 0 ; i < servers.size() ; ++i) {
        if (servers[i]->get_socket_fd() == client_fd) {
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

void    ft_manage_answer(Request* request, ConnectionInfo connection)
{
    int answ = request->get_is_ready();
    std::cout << "answer :" << answ << std::endl;
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

void    run_epoll(int epoll_fd, std::vector<Server*> servers)
{
    epoll_event events[MAX_EVENTS];
    // vector de connections 
    //std::vector<ConnectionInfo> connections;

    std::cout << GREEN << "WAITING FOR THE FIRST REQUEST...\n" << RESET;
    while (true) {
        print_listen(servers);

        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        
        if (event_count < 0) {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            if (events[i].events & EPOLLIN) {
                int client_fd = events[i].data.fd;
                // rajouter une connection dans le tableau de connections if it is a new connection
                ConnectionInfo client_connection = find_connection(client_fd, servers);  // the new connection should be added to the epoll list
                // Connection should contain a request object
                // Start parsing request or continue parsing if the parsing is not done
                
                Request* request = get_data_from_connection(client_connection); // a l'interieur de cette fonction, on fait un seul read et puis on passe a la prochaine connection
                
                // only if request is ready.... manage answer and close connection

                ft_manage_answer(request, client_connection); 
                // remove connection from epoll list if the request is done
                client_connection.connection->close(); // Georg add: Else the client is still open and it continues to send or wait for data
                delete client_connection.connection;
            }
        }
        std::cout << GREEN << "WAITING FOR THE NEXT REQUEST...\n" << RESET;
    }
}