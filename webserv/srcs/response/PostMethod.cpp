
#include "WebServ.hpp"

Response handle(const Request& request)
{
    // Handle POST request
    // Process submitted data
    // Return Response object
    /* EXEMPLE */
    
    // Process the request
    if (_is_valid_data(data)) {
        newResource = createResource(data)
        return (Response(201, "Created", newResource));
    }
    else {
        return (Response(400, "Bad Request"));
    }
}
