
#include "../../includes/Cgi.hpp"


CGI::CGI(std::string name, std::string compiler_path, std::string extension, int time_out){
    this->_name = name;
    this->_compiler_path = compiler_path;
    this->_extension = extension;
    this->_time_out = time_out;
}
        
std::string CGI::get_name(void){
    return (this->_name);
}

std::string CGI::get_path(void){
    return (this->_compiler_path);
}
        
std::string CGI::get_extension(void){
    return (this->_extension);
}

int CGI::get_time_out(void){
    return (this->_time_out);
}

// Fonction de nettoyage isspace
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// atoi a string
static int parse_int(const char *str) {
    return atoi(str);
}


static CGI get_cgi(std::ifstream& file, std::string line){
    static int  i = 0;

    std::string _name = "\0"; //Stores the CGI's name
    std::string _compiler_path = "\0"; //path of the cgi
    std::string _extension = "\0"; //py || php || js || ...
    int _time_out = 0;
    
    std::getline(file, line);
    trim(line);

    while (line[0] != '}'){
        if (line.find("cgi_path ") != std::string::npos && line.size() > 1) {
            line = line.substr(10);
            trim(line);
            if (line.size() > 0)
                _compiler_path = line;
        }
        else if (line.find("cgi_extension ") != std::string::npos && line.size() > 1) {
            line = line.substr(15);
            trim(line);
            if (line.size() > 0)
                _extension = line;
        }
        else if (line.find("cgi_timeout ") != std::string::npos){
            line = line.substr(13);
            trim(line);
            if (line.size() > 0)
                _time_out = parse_int(line.c_str());
        }
        else if (line.find("language ") != std::string::npos){
            line = line.substr(15);
            trim(line);
            if (line.size() > 0)
                _name = line;
        }
        if(!std::getline(file, line))
            break;
        trim(line);
    }

    i++;
    if (_name != "\0" && _compiler_path != "\0" && _extension != "\0" && _time_out > 0){
        return (CGI(_name, _compiler_path, _extension, _time_out));
    }
    else {
        std::cerr << "The cgi number " << i << " of your .config has something wrong:\n";
        if (_name != "\0")
            std::cerr << "- It doesn't have a name\n";
        if (_compiler_path != "\0")
            std::cerr << "- It doesn't have a compiler path\n"; 
        if (_extension != "\0")
            std::cerr << "- It doesn't have an extension type\n";
        if (_time_out <= 0)
            std::cerr << "- It doesn't have a valid time_out\n";
        std::cerr << std::endl;
        return (CGI("\0", "\0", "\0", 0));
    }
}

std::vector<CGI > parse_cgis(std::ifstream& file, std::string line){
    std::vector<CGI> current_config;
    std::getline(file, line);
    trim(line);

    while (line[0] != '}'){
        if (line.find("{") != std::string::npos && line.size() > 1 && line[0] != '{') {
            CGI tmp = get_cgi(file, line);
            current_config.push_back(tmp);
        }
        if(!std::getline(file, line))
            break;
        trim(line);
    }
    return (current_config);
}

//Inside location {}
static  Route_config get_route(std::ifstream& file, std::string line)
{
    Route_config current_route;
    std::getline(file, line);
    trim(line);
    while (line[0] != '}'){
        if (line.find("methods ") != std::string::npos){
            line = line.substr(9);
            std::string new_method;
            for (size_t i = 0; i < line.length(); ++i) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r'){
                    current_route.accepted_methods.push_back(new_method);
                    new_method = '\0';
                    i++;
                }
                new_method += line[i];
            }
        }
        else if (line.find("root ") != std::string::npos) {
            current_route.root_dir = line.substr(6);
        }
        else if (line.find("return ") != std::string::npos){
            line = trim(line.substr(7));
            current_route.redirection_nb = parse_int(line.c_str());
            if (current_route.redirection_nb != 301 && current_route.redirection_nb != 302 
                && current_route.redirection_nb != 303 && current_route.redirection_nb != 307
                && current_route.redirection_nb != 308){
                    
                std::cerr << current_route.redirection_nb << " is not a valid return number\n";
                current_route.redirection_nb = 0;
            }
            else {
                line = trim(line.substr(3));
                current_route.redirection_path = line;
            }
        }
        else if (line.find("autoindex ") != std::string::npos) {
            line = line.substr(11);
            current_route.dir_listing = line == "ON;" || line=="on;";
        }
        else if (line.find("use_cgi ") != std::string::npos) {
            line = line.substr(9);
            current_route.use_cgi = line == "YES;" || line == "yes;";
        }
        else if (line.find("index ") != std::string::npos) {
            current_route.default_file = line.substr(7);
        }
        else if (line.find("upload_store ") != std::string::npos) {
            current_route.upload_dir = line.substr(14);
        }
        if(!std::getline(file, line))
            break;
        trim(line);
    }
    return (current_route);
}

static Config_data parse_server(std::ifstream& file, std::string line)
{
    Config_data current_config;
    std::getline(file, line);
    trim(line);
    while (line[0] != '}')
    {
        if (line.empty() || line[0] == '#')
            continue ;
                // End of a server block

        // Parse the listen directive
        else if (line.find("listen ") != std::string::npos) {
            line = trim(line.substr(7));
            current_config.port = parse_int(line.c_str());
        }
        else if (line.find("host ") != std::string::npos) {
            line = trim(line.substr(6));
            current_config.host = line;
        }
                // Parse the server_name directive
        else if (line.find("server_name ") != std::string::npos) {
            line = trim(line.substr(13));
            current_config.server_name = line;
        }
        else if (line.find("error_page ") != std::string::npos){
            line = trim(line.substr(12));
            current_config.error_pages = line;
        }
        else if (line.find("directory_page ") != std::string::npos){
            line = trim(line.substr(16));
            current_config.error_pages = line;
        }
                // Parse the client_max_body_size directive
        else if (line.find("client_max_body_size ") != std::string::npos) {
            line = trim(line.substr(21));
            current_config.client_body_size_limit = parse_int(line.c_str());
        }

        // Parse the location /cgi-bin directive
        else if (line.find("location ") != std::string::npos && line.find("{")) {
            std::string name = line.substr(10);
            name = name.substr(0, name.size() - 1);
            Route_config new_route = get_route(file, line);
            current_config.routes.insert(std::pair<std::string, Route_config>(name,new_route));
        }
        if(!std::getline(file, line))
            break;
        trim(line);
    }
    // check_config(current_config);
    return (current_config);
}

void    print(std::vector<Config_data> data, std::vector<CGI> cgi){
    for (size_t i = 0; i < data.size(); i++){
        std::cout
            << "host, " << i << " : " << data[i].host << std::endl
            << "port, " << i << " : " << data[i].port << std::endl
            << "name, " << i << " : " << data[i].server_name << std::endl
            << "is a default server, " << i << " : " << data[i].is_default_server << std::endl
            << "error pages, " << i << " : " << data[i].error_pages << std::endl
            // << "method pages, " << i << " : " << data[i].method_pages << std::endl
            << "body_size, " << i << " : " << data[i].client_body_size_limit << std::endl;
        
        std::map<std::string, Route_config> routes = data[i].routes;
        std::cout << "In routes :\n";
        std::map<std::string, Route_config>::iterator it;
        for (it = routes.begin(); it != routes.end(); ++it){
            std::cout
                << "Route name : " << it->first << std::endl
                << "In config:\n\t-root directory : " << it->second.root_dir 
                << "\n\t-redirection :"
                << "\n\t-dir_listing :" << it->second.dir_listing
                << "\n\t-use of cgi :" << it->second.use_cgi
                << "\n\t-default files :" << it->second.default_file
                << "\n\t-upload directory :" << it->second.upload_dir
                << "\n\t-redirection_nb : " << it->second.redirection_nb
                << " ; redirection_path : " << it->second.redirection_path 
                << "\n\t-accepted methods : " ;
                for (size_t j = 0; j < it->second.accepted_methods.size() ; j++){
                    std::cout << it->second.accepted_methods[j] << " ; ";
                }

                // std::map<int, std::string>::iterator it2;
                // for (it2 = it->second.redirection.begin(); it2 != it->second.redirection.end(); ++it){
                //     std::cout << "nb:" << it2->first << ", path: " << it2->second << ";" ;
                // }
        }
    }
//ICI
    for (size_t i = 0; i < cgi.size(); i++){
        std::cout 
            << "CGI :"
            << "\n\tName : " << cgi[i].get_name() 
            << "\n\tPath : " << cgi[i].get_path()
            << "\n\tExtension : " << cgi[i].get_extension()
            << "\n\tTime out : " << cgi[i].get_time_out() 
            << std::endl;
    }
}

// Fonction principale pour parser le fichier de configuration
std::vector<Config_data> parse_config(const char *filename) {
    std::vector<Config_data> configs;
    std::vector<CGI> cgi;
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error(std::string("Error opening file: ") + strerror(errno));        return (configs);
    }

    std::string line;

    while (std::getline(file, line)) {
        
        // Process each line in the buffer
            trim(line);

            // Start of a new server block
            if (line.find("server {") != std::string::npos) {
                Config_data current_config;
                current_config = parse_server(file, line); 
                configs.push_back(current_config);
            }
            else if (line.find("cgi {") != std::string::npos) {
                cgi = parse_cgis(file, line); 
            }
            else
                continue;
    }    
    file.close();
    print(configs, cgi);
    return (configs);
}

