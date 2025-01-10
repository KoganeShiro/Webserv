#include "WebServ.hpp"
// #include "../includes/Config.hpp"
// #include "../includes/Cgi.hpp"
// #include "../includes/Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
// #include "../includes/Socket.hpp"
// #include "../includes/DeleteMethod.hpp"
// #include "../includes/GetMethod.hpp"
// #include "../includes/PostMethod.hpp"
#include "Worker.hpp"
// #include "Connection.hpp"


// Response ft_worker_response(Config_data data, Request request)
// {
//     Worker worker(data, request);

//     return (worker.run());
// }
struct ConnectionInfo {
    Connection* connection = NULL;
    Config_data data;
};

Request *get_data_from_connection(ConnectionInfo client_connection) {
    std::cout << client_connection.connection << std::endl;
    int client_fd = client_connection.connection->get_clientfd();
    std::cout << "client fd = " << client_fd << std::endl;
    
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
        // std::cout << "Octets reçus : " << bytes_received << std::endl;
        buffer[bytes_received] = '\0';
        data.append(buffer);
        std::cout << "data : " << data << std::endl;
        answer = request->add_to_request(data,client_connection.data.client_body_size_limit);
        std::cout << "ANSWER = " << answer << std::endl;
        if (answer == BAD_HEADER || answer == GOOD)
            break;
    }
    if (bytes_received < 0) {
        std::cerr << "Erreur de lecture socket du client : " << client_fd << std::endl;
        return (NULL);
    }
    std::cout << "End of read\n";
    return (request);
}


ConnectionInfo find_connection(int client_fd, std::vector<Server*> servers){
    ConnectionInfo ci;
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
        ssize_t bytes_sent = send(connection.connection->get_clientfd(), &str, str.length(), 0);
        if (bytes_sent == -1) {
            perror("send");
        }
    }
}

void    run_epoll(int epoll_fd, std::vector<Server*> servers)
{
    epoll_event events[MAX_EVENTS];

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
                // Request* request = client_connection.connection->get_request();
                Request* request = get_data_from_connection(client_connection);
                std::cout << "ICI\n";
                // request->add_to_request(str_buff, client_connection.data.client_body_size_limit);
                ft_manage_answer(request, client_connection);
                delete client_connection.connection;
            }
        }
    }
}

void add_to_epoll(int epoll_fd, Server *server)
{
    epoll_event event;
    event.data.fd = server->get_socket_fd(); // Associer le fd du socket
    if (event.data.fd < 0) {
        std::cerr << "Invalid socket file descriptor: " << event.data.fd << std::endl;
        throw std::runtime_error("Invalid socket file descriptor");
    }
    event.events = EPOLLIN;
    if (fcntl(event.data.fd, F_GETFD) == -1) {
        std::cerr << "Socket fd is closed or invalid. Error: " << strerror(errno) << std::endl;
        throw std::runtime_error("Socket fd is closed or invalid.");
    }

    std::cerr << "epoll_fd: " << epoll_fd << ", socket_fd: " << event.data.fd << std::endl;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) < 0) {
        std::cerr << "Failed to add socket to epoll: " << strerror(errno) << std::endl;
        perror("Error details");
        std::cerr << "epoll_fd: " << epoll_fd << ", socket_fd: " << event.data.fd << std::endl;
        close(epoll_fd);
        throw std::runtime_error("Failed to add socket to epoll");
    }
    std::cout << "Debug worked\n";
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            "Usage: ./WebServ <configuration file>"
        << std::endl;
        return(EXIT_FAILURE) ;
    }
    else {
        // std::cout <<
        //     EXPLAINATION 
        // << std::endl;
    }

    // 1. Parser le fichier de configuration
    try {
        std::vector<Config_data> configs = parse_config(argv[1]);
       
        // 2. Creer les instances de serveurs
        std::vector<Server> servers;
        for (size_t i = 0; i < configs.size(); ++i) {
            servers.push_back(Server(configs[i]));
        }
        std::cout << servers.size();
        // // 3. Creer une instance epoll
        // int epoll_fd = epoll_create(1);
        // if (epoll_fd < 0) {
        //     std::cerr << "epoll_create failed\n";
        //     return EXIT_FAILURE;
        // }

        // 2. Creer une instance epoll
        int epoll_fd = epoll_create(1);
        if (epoll_fd < 0) {
            std::cerr << "epoll_create failed\n";
            return (EXIT_FAILURE);
        }

        // 3. Creer les instances de serveurs
        std::vector<Server*> servers;
        for (size_t i = 0; i < configs.size(); ++i) {
            std::cout << "avant\n";
            servers.push_back(new Server(configs[i]));
            std::cout << "Apres\n";
        }
        std::cout << servers.size() << std::endl;
        if (servers.size() <= 0)
            throw std::runtime_error(std::string("Your file '.config' doesn't configure a server as requested\n"));
        // std::cout << "Socket FD: " << (*servers[0]).get_socket_fd() << std::endl;
        // std::cout << "Epoll FD: " << epoll_fd << std::endl;  

        // // // 4. Ajouter les sockets des serveurs à epoll
        for (size_t i = 0; i < servers.size(); ++i) {
            add_to_epoll(epoll_fd, servers[i]);
        }

        // // // 5. Demarrer la boucle pour gerer les events
        run_epoll(epoll_fd, servers);

        // // Nettoyage
        // close(epoll_fd);
        // for (size_t i = 0; i < servers.size(); ++i) {
        //     servers[i].shutdown(); // Ajouter une methode pour fermer proprement
        // }

        return (EXIT_SUCCESS);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}