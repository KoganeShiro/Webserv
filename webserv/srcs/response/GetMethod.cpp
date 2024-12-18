
#include "WebServ.hpp"

Response handle(const Request& request) {
    // Handle GET request
    // Read file or generate content
    // Return Response object
    /* EXEMPLE */

    Response response(200, "OK");
    
    // Process the request
    std::string content = open(html_file);
    
    std::vector<char> body(content.begin(), content.end());
    response.setBody(body);
    response.setHeader("Content-Type", "text/html");
    
    return (response);

}
