#include "WebServ.hpp"
#include "Worker.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Cgi.hpp"
#include "Server.hpp"


/*
Config_data hard_code(Config_data *config, Route_config *route)
{
    // Server configuration
    config->host = "127.0.0.1";
    config->port = 8080;
    config->server_name ="localhost";    
    config->is_default_server = true;

    // Error pages
    config->error_pages = "html_page/template/error/index.html";
    // config->method_pages = "html_page/template/method_success/index.html";
    config->directory_page = "html_page/template/directory_listing/directory.html";

    // Client body size limit (10MB)
    config->client_body_size_limit = 10 * 1024 * 1024;

    // Route configuration
   // config->route_config = route;
    route->accepted_methods.push_back("GET");
    route->accepted_methods.push_back("POST");
    route->accepted_methods.push_back("DELETE");
    route->root_dir = "html_page/www/html";
    route->dir_listing = true;
    route->default_file = "index.html";
    route->use_cgi = true;
 //   route->upload_dir = "html_page/www/html";
    route->redirection_path = "https://www.google.com";
    route->redirection_nb = 0 ;

    config->routes["/"] = *route;

    //config->tab_cgi = new std::vector<CGI>;

    // CGI configuration
    //config->cgi_path = "/usr/bin/php-cgi";
   CGI cgi("bla", "testers/ubuntu_cgi_tester", "bla", 10);

  config->tab_cgi.push_back(cgi); 
    return (*config);
}

Config_data config_parser(const std::string config)
{
    Config_data config_data;
    Route_config route;
    
    (void) config;
    //open file
    // Read and parse configuration file
    // Return Config struct
    return (hard_code(&config_data, &route));
}

void printconfig(Config_data& config)
{
    std::cout << "Host: " << config.host << std::endl;
    std::cout << "Port: " << config.port << std::endl;
    std::cout << "Server name: " << config.server_name << std::endl;
    std::cout << "Default server: " << config.is_default_server << std::endl;
    std::cout << "Error pages: " << config.error_pages << std::endl;
    std::cout << "Directory page: " << config.directory_page << std::endl;
    std::cout << "Client body size limit: " << config.client_body_size_limit << std::endl;
    std::cout << "Route configuration:" << std::endl;
    std::cout << "Accepted methods: ";
    for (size_t i = 0; i < config.routes["/"].accepted_methods.size(); ++i) {
        std::cout << config.routes["/"].accepted_methods[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Root directory: " << config.routes["/"].root_dir << std::endl;
    std::cout << "Directory listing: " << config.routes["/"].dir_listing << std::endl;
    std::cout << "Default file: " << config.routes["/"].default_file << std::endl;
    std::cout << "Use CGI: " << config.routes["/"].use_cgi << std::endl;
    std::cout << "Redirection path: " << config.routes["/"].redirection_path << std::endl;
    std::cout << "Redirection number: " << config.routes["/"].redirection_nb << std::endl;
    std::cout << "CGI configuration:" << std::endl;
    std::cout << "tabcgisize: " << config.tab_cgi.size() << std::endl;
    for (size_t i = 0; i < config.tab_cgi.size(); ++i) {
        std::cout << "Language: " << config.tab_cgi[i].get_name() << std::endl;
        std::cout << "Path: " << config.tab_cgi[i].get_path() << std::endl;
        std::cout << "Extension: " << config.tab_cgi[i].get_extension() << std::endl;
        std::cout << "Timeout: " << config.tab_cgi[i].get_time_out() << std::endl;
    }
 
}
*/


int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout <<
            "Usage: ./WebServ <configuration file>"
        << std::endl;
        return(EXIT_FAILURE) ;
 
   }
   

    Config_data config;
    // 1. Parser le fichier de configuration
    try {
        std::vector<Config_data> configs = parse_config(argv[1]);
       
        // 2. Creer les instances de serveurs
        std::vector<Server> servers;
        for (size_t i = 0; i < configs.size(); ++i) {
            servers.push_back(Server(configs[i]));
        }
     //   std::cout << servers.size();
     //   printconfig(configs[0]);
        config = configs[0];
      
       // return (EXIT_SUCCESS);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout <<
             EXPLAINATION 
         << std::endl;


    }



  // Config_data config = config_parser("test.config");
    Request *request = new Request();

    request->set_path("/youpi.bla/youpi?name=123");
    request->set_method("GET");
    request->set_body("Hello, World!");
    request->add_header("Host", "localhost");
    request->add_header("Content-Type", "text/html");
    request->add_header("Content-Length", "13");
    
    Worker worker(config, request);
    Response response = worker.run();
    std::cout << response.http_response() << std::endl;   
    
    return 0;
}
