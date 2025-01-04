
#include "WebServ.hpp"

void print_Config_data(Config_data data)
{
    //print ServConfig
    std::cout << "Server Configuration:" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Host: " << data.host << std::endl;
    std::cout << "Port: " << data.port << std::endl;
    
    std::cout << "Server Names: ";
    //vector or simple std::string ?
    std::cout << std::endl;
    
    std::cout << "Is Default Server: " << (data.is_default_server ? "Yes" : "No") << std::endl;
    std::cout << "Error Pages: " << data.error_pages << std::endl;
    std::cout << "Method Pages: " << data.method_pages << std::endl; //not necessary
    std::cout << "Client Body Size Limit: " << data.client_body_size_limit << std::endl;
    
    std::cout << "\nRoutes Configuration:" << std::endl;
    std::cout << "---------------------" << std::endl;
    for (std::map<std::string, Route_config>::const_iterator it = data.routes.begin(); it != data.routes.end(); ++it) {
        std::cout << "Route: " << it->first << std::endl;
        std::cout << "  Accepted Methods: ";
        for (size_t i = 0; i < it->second.accepted_methods.size(); ++i) {
            std::cout << it->second.accepted_methods[i];
            if (i < it->second.accepted_methods.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        std::cout << "  Redirection: " << it->second.redirection << std::endl;
        std::cout << "  Root Directory: " << it->second.root_dir << std::endl;
        std::cout << "  Directory Listing: " << (it->second.dir_listing ? "Enabled" : "Disabled") << std::endl;
        std::cout << "  Default File: " << it->second.default_file << std::endl;
        std::cout << "  CGI Extension: " << it->second.cgi_extension << std::endl;
        std::cout << "  Upload Directory: " << it->second.upload_dir << std::endl;
    }
    
    std::cout << "\nCGI Configuration:" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "CGI Path: " << data.cgi_path << std::endl;
    if (data.tab_cgi != NULL) {
        std::cout << "Number of CGIs: " << data.tab_cgi->nb_cgi << std::endl;
        // Assuming CGI struct has a name or identifier to print
        for (int i = 0; i < data.tab_cgi->nb_cgi; ++i) {
            std::cout << "  CGI " << i + 1 << ": " << data.tab_cgi->tab_cgi[i].name << std::endl;
        }
    } else {
        std::cout << "No CGI configuration available." << std::endl;
    }
}

static int _is_file_config(const std::string& file)
{
    if (file.size() >= 7 && file.compare(file.size() - 7, 7, ".config") == 0) {
        std::cout << "\033[1m\033[32mfile is a .config\033[0m\n";
        return EXIT_SUCCESS;
    }
    std::cout << "\033[1m\033[31mfile is not a .config\033[0m\n";
    return EXIT_FAILURE;
}

static bool is_comment_or_empty(const std::string& line)
{
    if (line.find_first_not_of(" \t") == std::string::npos)
        return (true);
    
    size_t first_non_space = line.find_first_not_of(" \t");
    return (line[first_non_space] == '#');
}

static int fill_struct(const std::string& line, Config_data* config)
{
    // Init/fill the Config_data struct
    // Return EXIT_FAILURE if parsing fails, EXIT_SUCCESS otherwise
    return (EXIT_SUCCESS);
}

static std::string get_line(int fd)
{
    std::string line;
    char c;

    while (read(fd, &c, 1) > 0) {
        if (c == '\n')
            break;
        line += c;
    }
    return (line);
}

static int read_file(int fd, Config_data* config)
{
    std::string line;
    while (!(line = get_line(fd)).empty()) {
        if (is_comment_or_empty(line)) {
            continue;
        }

        if (fill_struct(line, config) == EXIT_FAILURE) {
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

int parse_file(const std::string& file, Config_data* config)
{
    if (_is_file_config(file) == EXIT_FAILURE) {
        return (EXIT_FAILURE);
    }

    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0) {
        perror(file.c_str());
        return (EXIT_FAILURE);
    }

    if (_read_file(fd, config) == EXIT_FAILURE) {
        close(fd);
        return (EXIT_FAILURE);
    }

    close(fd);
    return (EXIT_SUCCESS);
}

int test(const std::string& file)
{
    Config_data* config = new Config_data();

    std::cout << "\033[1m\033[37m========= " << file << " =========\033[0m\n";
    if (parse_file(file, config) == EXIT_FAILURE) {
        std::cout << "\033[31mError \033[0m\n\n";
        delete config;
        return EXIT_FAILURE;
    } else {
        std::cout << "\033[32mSuccess \033[0m\n\n";
        delete config;
        return EXIT_SUCCESS;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            RED "Usage: ./WebServ <configuration file>" RESET
        << std::endl;
        return ;
    }
    const std::string config = argv[1];
    test(config);
    
    return (0);
}