
#include "WebServ.hpp"

static int _is_file_config(const std::string& file) {
    if (file.size() >= 7 && file.compare(file.size() - 7, 7, ".config") == 0) {
        std::cout << "\033[1m\033[32mfile is a .config\033[0m\n";
        return EXIT_SUCCESS;
    }
    std::cout << "\033[1m\033[31mfile is not a .config\033[0m\n";
    return EXIT_FAILURE;
}

static char* get_next_line(int fd) {
    std::string line;
    char c;
    while (read(fd, &c, 1) > 0) {
        if (c == '\n') break;
        line += c;
    }
    if (line.empty()) return NULL;
    char* result = new char[line.size() + 1];
    std::strcpy(result, line.c_str());
    return result;
}

static int fill_struct(const std::string& line, size_t start_idx, Config_data* config) {
    // Fill in the Config_data structure based on your line processing
    // Returning EXIT_SUCCESS for now; replace with your logic
    return EXIT_SUCCESS;
}

static int _read_rtfile(int fd, Config_data* config) {
    char* line = get_next_line(fd);
    while (line != NULL) {
        std::string str_line(line);
        delete[] line;

        size_t start_idx = str_line.find_first_not_of(" \t\n\r");
        if (start_idx == std::string::npos) {
            line = get_next_line(fd);
            continue;
        }

        if (fill_struct(str_line, start_idx, config) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }

        line = get_next_line(fd);
    }
    return EXIT_SUCCESS;
}

int parse_file(const std::string& file, Config_data* config) {
    if (_is_file_config(file) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0) {
        perror(file.c_str());
        return EXIT_FAILURE;
    }

    if (_read_rtfile(fd, config) == EXIT_FAILURE) {
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}

int test(const std::string& file) {
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