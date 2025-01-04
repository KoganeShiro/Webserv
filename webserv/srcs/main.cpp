// #include "../includes/WebServ.hpp"
// #include "../includes/Config.hpp"
#include "../includes/Cgi.hpp"
// #include "../includes/Request.hpp"
// #include "../includes/Response.hpp"
// #include "../includes/Server.hpp"
// #include "../includes/Socket.hpp"
// #include "../includes/DeleteMethod.hpp"
// #include "../includes/GetMethod.hpp"
// #include "../includes/PostMethod.hpp"
// #include "../includes/Worker.hpp"
// #include "../includes/Connection.hpp"

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


// Response ft_worker_response(Config_data data, Request request)
// {
//     Worker worker(data, request);

//     return (worker.run());
// }

//---- AJOUT DAMIEN ----
// void    run_epoll(int epoll_fd, std::vector<Server> servers)
// {
//     epoll_event events[MAX_EVENTS];
//     while (true) {
//         int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
//         if (event_count < 0) {
//             perror("epoll_wait failed");
//             break;
//         }

//         for (int i = 0; i < event_count; ++i) {
//             int event_fd = events[i].data.fd;

//             // Vérifier si l'événement concerne un socket serveur
//             bool is_server_socket = false;
//             for (size_t j = 0; j < servers.size(); ++j) {
//                 if (servers[j].get_socket_fd() == event_fd) {
//                     is_server_socket = true;
//                     break;
//                 }
//             }

//             if (is_server_socket) {
//                 // Nouvelle connexion sur un socket serveur
//                 for (size_t j = 0; j < servers.size(); ++j) {
//                     if (servers[j].get_socket_fd() == event_fd) {
//                         servers[j].accept_connection(epoll_fd); // Accepter la connexion et gérer
//                         break;
//                     }
//                 }
//             } else {
//                 // Événement sur un socket client
//                 char buffer[1024] = {0};
//                 ssize_t bytes_read = read(event_fd, buffer, sizeof(buffer));
//                 if (bytes_read <= 0) {
//                     // Déconnexion ou erreur
//                     close(event_fd);
//                     epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event_fd, NULL);
//                     std::cout << "Client disconnected\n";
//                 } else {
//                     // Traiter les données reçues
//                     std::cout << "Received: " << buffer << "\n";
//                     const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello World!";
//                     send(event_fd, response, strlen(response), 0);
//                 }
//             }
//         }
//     }
// }
// std::vector<Config_data> parse_configs(const char *filename) {
//     std::vector<Config_data> configs;
//     std::ifstream file(filename);
//     if (!file) {
//         throw std::runtime_error(std::string("Error opening file: ") + strerror(errno));        return (configs);
//     }

//     std::string line;

//     while (std::getline(file, line)) {
        
//         // Process each line in the buffer
//             trim(line);

//             // Start of a new server block
//             if (line.find("server {") != std::string::npos) {
//                 Config_data current_config;
//                 current_config = parse_server(file, line); 
//                 configs.push_back(current_config);
//             }
//             else if (line.find("cgi {") != std::string::npos) {
//                 current_config.tab_cgi = parse_cgis(file, line); 
//             }
//             else
//                 continue;
//     }    
//     file.close();
//     return (configs);
// }

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            "Usage: ./WebServ <configuration file>"
        << std::endl;
        return(EXIT_FAILURE) ;
    }
    else{
        std::cout <<
            EXPLAINATION 
        << std::endl;
    }

    // 1. Parser le fichier de configuration
    try {
        std::vector<Config_data> configs = parse_config(argv[1]);
       
        // 2. Creer les instances de serveurs
        // std::vector<Server> servers;
        // for (size_t i = 0; i < configs.size(); ++i) {
        //     servers.push_back(Server(configs[i]));
        // }

        // // 3. Creer une instance epoll
        // int epoll_fd = epoll_create(1);
        // if (epoll_fd < 0) {
        //     std::cerr << "epoll_create failed\n";
        //     return EXIT_FAILURE;
        // }

        // // 4. Ajouter les sockets des serveurs à epoll
        // for (size_t i = 0; i < servers.size(); ++i) {
        //     servers[i].add_to_epoll(epoll_fd);
        // }

        // // 5. Demarrer la boucle pour gerer les events
        // run_epoll(epoll_fd, servers);

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


//----FIN AJOUT DAMIEN---

// int main(int argc, char **argv)
// {
//     if (argc != 2) {
//         std::cout <<
//             RED "Usage: ./WebServ <configuration file>" RESET
//         << std::endl;
//         return ;
//     }
//     // setup_server(argv[1]); //configuration parsing
//         //Server server(setup_server(argv[1]));



//     // listen_on_socket(); //will call request_parser()
//     // worker_response(); //send the response of the request
    
// // Load configuration
//     const std::string config = argv[1];
//     Server server(config);
// // Initialize server
//     server.run();
    
//     return (0);
// }