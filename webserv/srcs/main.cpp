#include "WebServ.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Worker.hpp"

void    run_epoll(int epoll_fd, std::vector<Server*> servers);
std::vector<Server*> ServerManager::servers;

void signalHandler(int signum)
{
    for (size_t i = 0; i < ServerManager::servers.size(); ++i)
        close(ServerManager::servers[i]->get_socket_fd());

    const char *argv[] = {"./free", NULL};
    execve("./free", (char *const *)argv, NULL);
    std::cerr << RED << "Couldn't find 'free'\n";
    exit(signum);
}

void add_to_epoll(int epoll_fd, Server *server)
{
    epoll_event event;
    event.data.fd = server->get_socket_fd(); // Associate the socket file descriptor
    if (event.data.fd < 0) {
        std::cerr << "Invalid socket file descriptor: " << event.data.fd << std::endl;
        throw std::runtime_error("Invalid socket file descriptor");
    }
    event.events = EPOLLIN;
    if (fcntl(event.data.fd, F_GETFD) == -1) {
        std::cerr << "Socket fd is closed or invalid. Error: " << strerror(errno) << std::endl;
        throw std::runtime_error("Socket fd is closed or invalid.");
    }
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) < 0) {
        std::cerr << "Failed to add socket to epoll: " << strerror(errno) << std::endl;
        perror("Error details");
        std::cerr << "epoll_fd: " << epoll_fd << ", socket_fd: " << event.data.fd << std::endl;
        close(epoll_fd);
        throw std::runtime_error("Failed to add socket to epoll");
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            RED "Usage: ./WebServ <configuration file>" RESET
        << std::endl;
        return(EXIT_FAILURE);
    }
    else {
        std::cout <<
            EXPLAINATION 
        << std::endl;
    }

    // 1. Parser le fichier de configuration
    try {
        signal(SIGINT, signalHandler);
        std::vector<Config_data> configs = parse_config(argv[1]);

        // 2. Creer une instance epoll
        int epoll_fd = epoll_create(1);
        if (epoll_fd < 0) {
            std::cerr << "epoll_create failed\n";
            return (EXIT_FAILURE);
        }

        // 3. Creer les instances de serveurs
        // std::vector<Server*> servers;
        for (size_t i = 0; i < configs.size(); ++i) {
            ServerManager::servers.push_back(new Server(configs[i]));
        }
        if (ServerManager::servers.size() <= 0)
            throw std::runtime_error(std::string(BAD_CONFIG));

        // 4. Ajouter les sockets des serveurs a epoll
        for (size_t i = 0; i < ServerManager::servers.size(); ++i) {
            add_to_epoll(epoll_fd, ServerManager::servers[i]);
        }
        // 5. Demarrer la boucle pour gerer les events
        run_epoll(epoll_fd, ServerManager::servers);

        return (EXIT_SUCCESS);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}