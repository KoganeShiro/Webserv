#include "WebServ.hpp"
#include "Worker.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config_data.hpp"



Config_data hard_code(Config_data *config, Route_config *route)
{
    // Server configuration
    config->host = "127.0.0.1";
    config->port = 8080;
    config->server_names.push_back("localhost");    
    config->is_default_server = true;

    // Error pages
    config->error_pages = "/html_page/template/error/index.html";
    config->method_pages = "/html_page/template/method_success/index.html";

    // Client body size limit (10MB)
    config->client_body_size_limit = 10 * 1024 * 1024;

    // Route configuration
   // config->route_config = route;
    route->accepted_methods.push_back("GET");
    route->accepted_methods.push_back("POST");
    route->accepted_methods.push_back("DELETE");
    route->root_dir = "/html_page/www/html";
    route->dir_listing = true;
    route->default_file = "index.html";
    route->cgi_extension = ".php";
    route->upload_dir = "/html_page/www/uploads";

    config->routes["/"] = *route;


    // CGI configuration
    config->cgi_path = "/usr/bin/php-cgi";
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
    request->set_path("/");
    request->set_method("GET");
    request->add_header("Host", "localhost");
//    request.set_body("Hello, World!");
//    request.set_headers("Content-Type", "text/html");
//    request.set_headers("Content-Length", "13");
    
    Worker worker(config, request);
    Response response = worker.run();
    std::cout << response.http_response() << std::endl;
    return 0;
}
