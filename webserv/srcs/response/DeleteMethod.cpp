
#include "WebServ.hpp"

Response handle(const Request& request) {
    // Handle DELETE request
    // Delete resource if exists and permitted
    // Return Response object
    /* EXEMPLE */
    
    // Process the request
    if resourceExists(resource) {
        deleteResource(resource)
        return (Response(204, "No Content")) //successfully delete
    }
    else {
        return (Response(404, "Not Found"))
    }
}
