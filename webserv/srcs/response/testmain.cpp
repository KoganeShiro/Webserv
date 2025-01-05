#include "WebServ.hpp"
#include "Worker.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Cgi.hpp"



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
   CGI cgi("php", "testers/ubuntu_cgi_tester", "php", 10);

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



int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    Config_data config = config_parser("test.config");
    Request *request = new Request();
    request->set_path("/test.php?name=John&age=25");
    request->set_method("GET");
    request->set_body("Hello, World!");
    request->add_header("Host", "localhost");
    request->add_header("Content-Type", "text/html");
    request->add_header("Content-Length", "13");
//    request.set_body("Hello, World!");
//    request.set_headers("Content-Type", "text/html");
//    request.set_headers("Content-Length", "13");


    
    Worker worker(config, request);
    Response response = worker.run();
    std::cout << response.http_response() ;
   // delete config.tab_cgi;
    return 0;
}
