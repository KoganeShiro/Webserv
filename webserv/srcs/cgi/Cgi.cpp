#include "Cgi.hpp"

CGI::CGI(std::string name, std::string compiler_path,
            std::string extension, int time_out)
{
    this->_name = name;
    this->_compiler_path = compiler_path;
    this->_extension = extension;
    this->_time_out = time_out;
}
        
std::string CGI::get_name(void)
{
    return (this->_name);
}

std::string CGI::get_path(void)
{
    return (this->_compiler_path);
}
        
std::string CGI::get_extension(void)
{
    return (this->_extension);
}

int CGI::get_time_out(void)
{
    return (this->_time_out);
}

// Fonction de nettoyage isspace
static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return ("");  // La chaîne est vide ou ne contient que des espaces blancs
    }

    size_t last = str.find_last_not_of(" \t\n\r");
    return (str.substr(first, last - first + 1));  // Extrait la sous-chaîne entre first et last
}

// atoi a string
static int parse_int(const char *str)
{
    return (atoi(str));
}


static CGI get_cgi(std::ifstream& file, std::string line)
{
    std::string _name = "\0"; //Stores the CGI's name
    std::string _compiler_path = "\0"; //path of the cgi
    std::string _extension = "\0"; //py || php || js || ...
    int _time_out = 0;
    _name = trim(line.substr(0, line.find("{") - 1));
    std::getline(file, line);
    line = trim(line);

    while (line[0] != '}') {

        if (line.find("cgi_compiler ") != std::string::npos && line.size() > 13) {
            line = line.substr(13);
            line = trim(line);
            if (!line.empty()) {
                line.erase(line.size() - 1);
                _compiler_path = line;
            }
        }
        else if (line.find("cgi_extension ") != std::string::npos && line.size() > 1) {
            line = line.substr(14);
            line = trim(line);

            if (!line.empty()) {
                line.erase(line.size() - 1);
                _extension = line;
            }
        }
        else if (line.find("cgi_timeout ") != std::string::npos){
            line = line.substr(12);
            line = trim(line);

            if (!line.empty())
                _time_out = parse_int(line.c_str());
        }
        if(!std::getline(file, line))
            break;
        line = trim(line);
    }

    if (_compiler_path != "\0" && _extension != "\0" && _time_out > 0){
        return (CGI(_extension, _compiler_path, _extension, _time_out));
    }
    else {
        throw std::runtime_error(std::string("Something wrong with one of your CGI, check the explaination and fixe your file '.config'\n"));
    }
}

std::vector<CGI > parse_cgis(std::ifstream& file, std::string line)
{
    std::vector<CGI> current_config;
    std::getline(file, line);
    line = trim(line);

    while (line[0] != '}'){
        if (line.find("{") != std::string::npos && line.size() > 1 && line[0] != '{') {
            CGI tmp = get_cgi(file, line);
            current_config.push_back(tmp);
        }
        if(!std::getline(file, line))
            break;
        line = trim(line);
    }
    return (current_config);
}

//Inside location {}
static  Route_config get_route(std::ifstream& file, std::string line)
{
    Route_config current_route;
    std::getline(file, line);
    line = trim(line);

    while (line[0] != '}'){

        if (line.find("methods ") != std::string::npos){
            line = line.substr(8);
            line = trim(line);
            std::string new_method;
            for (size_t i = 0; i < line.length(); ++i) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r') {
                    if (!new_method.empty()) {
                        current_route.accepted_methods.push_back(new_method);
                        new_method.clear(); 
                    }
                } else {
                    new_method += line[i];
                }
            }
            if (!new_method.empty()) {
                new_method.erase(new_method.size() - 1);
                current_route.accepted_methods.push_back(new_method);            
            }
        }
        else if (line.find("root ") != std::string::npos) {
            line = line.substr(5);
            line.erase(line.size() - 1);
            current_route.root_dir = line;
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
                line.erase(line.size() - 1);
                current_route.redirection_path = line;
            }
        }
        else if (line.find("autoindex ") != std::string::npos) {
            line = line.substr(10);
            line = trim(line);
            current_route.dir_listing = (line == "ON;" || line=="on;") ? true : false;
        }
        else if (line.find("use_cgi ") != std::string::npos) {
            line = line.substr(8);
            line = trim(line);
            current_route.use_cgi = (line == "YES;" || line == "yes;") ? true : false;
        }
        else if (line.find("index ") != std::string::npos) {
            line = line.substr(6);
            line.erase(line.size() - 1);
            current_route.default_file = line;
        }
        if(!std::getline(file, line))
            break;
        line = trim(line);
    }
    return (current_route);
}

static Config_data parse_server(std::ifstream& file, std::string line)
{
    Config_data current_config;

    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.find("port ") != std::string::npos) {
            line = trim(line.substr(5));
            current_config.port = parse_int(line.c_str());
        }
        else if (line.find("host ") != std::string::npos) {
            line = trim(line.substr(5));
            line.erase(line.size() - 1);
            current_config.host = line;
        }
                // Parse the server_name directive
        else if (line.find("server_name ") != std::string::npos) {
            line = trim(line.substr(12));
            line.erase(line.size() - 1);
            current_config.server_name = line;
        }
        else if (line.find("error_page ") != std::string::npos){
            line = trim(line.substr(11));
            line.erase(line.size() - 1);
            current_config.error_pages = line;
        }
        else if (line.find("directory_page ") != std::string::npos){
            line = trim(line.substr(15));
            line.erase(line.size() - 1);
            current_config.directory_page = line;
        }
                // Parse the client_max_body_size directive
        else if (line.find("client_max_body_size ") != std::string::npos) {
            line = trim(line.substr(21));
            current_config.client_body_size_limit = parse_int(line.c_str());
            
            char    last_c = line[line.size() - 2];
            if (last_c == 'K'){
                current_config.client_body_size_limit *= 1000;
            }
            else if (last_c == 'M'){
                current_config.client_body_size_limit *= 1000000;
            }
            else if (last_c == 'G'){
                current_config.client_body_size_limit *= 1000000000;
            }
        }

        // Parse the location /cgi-bin directive
        else if (line.find("location ") != std::string::npos && line.find("{") ) {
            std::string name = line.substr(9);
            name = trim(name.substr(0, name.size() - 1));
            Route_config new_route = get_route(file, line);
            current_config.routes.insert(std::pair<std::string, Route_config>(name,new_route));
        }
        else if (line.find("}" ) != std::string::npos){
            break;
        }
    }
    // check_config(current_config);
    return (current_config);
}

void    print(std::vector<Config_data> data, std::vector<CGI> cgi)
{
    std::cout << "Print configs. Nb of servers in config: " << data.size() << std::endl;
    for (size_t i = 0; i < data.size(); i++){
        std::cout
            << "host, " << i << " : " << data[i].host << std::endl
            << "port, " << i << " : " << data[i].port << std::endl
            << "name, " << i << " : " << data[i].server_name << std::endl
            << "is a default server, " << i << " : " << data[i].is_default_server << std::endl
            << "error pages, " << i << " : " << data[i].error_pages << std::endl
            << "directory page, " << i << " : " << data[i].directory_page << std::endl
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
                << "\n\t-redirection_nb : " << it->second.redirection_nb
                << " ; redirection_path : " << it->second.redirection_path 
                << "\n\t-accepted methods : " ;
                for (size_t j = 0; j < it->second.accepted_methods.size() ; j++){
                    std::cout << it->second.accepted_methods[j] << " ; ";
                }
                std::cout << std::endl;

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

// void    ft_check_server(Config_data config)
// {
//     // int error = 0;

//     if (config.client_body_size_limit == 0)
//         config.client_body_size_limit = STD_BODY_SIZE;
//     if (config.error_pages == ""){

//     }
//     if (config.host == ""){
        
//     }
//     if (config.port < 0){

//     }
//     if (config.routes.size() == 0){

//     }
//     if (config.server_name == ""){

//     }
// }

// Fonction principale pour parser le fichier de configuration
std::vector<Config_data> parse_config(const char *filename)
{
    std::vector<Config_data> configs;
    std::vector<CGI> cgi;
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error(std::string("Error opening file: ") + strerror(errno));
    }
    std::string line;
    while (std::getline(file, line)) {

        
        // Process each line in the buffer
            line = trim(line);            
        // std::cout << "LINE => " << line << std::endl;
            // Start of a new server block
            if (line.find("server {") != std::string::npos) {                
                Config_data current_config;
                current_config = parse_server(file, line); 
                current_config.tab_cgi = cgi;
                configs.push_back(current_config);
            }
            else if (line.find("cgi {") != std::string::npos) {
                cgi = parse_cgis(file, line);
                 
            }

    }    
    file.close();
    // while
  //  print(configs, cgi);
    return (configs);
}
