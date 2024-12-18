
#include "WebServ.hpp"

void Server::run()
{
    while (true) { // Main loop to keep the server running
        try {
            Connection user_connect = this->socket.accept(); // Accept a new client connection
            handle_connection(user_connect); // Handle the accepted connection
        } catch (const std::exception& e) {
            std::cerr << "Error accepting connection: " << e.what() << std::endl;
            continue; // Continue to the next iteration of the loop
        }
    }
}

void Server::handle_connection(Connection user_connect)
{
    try {
        std::string request = user_connect.receive(); // Receive request from client
        //fill buffer until end of header (\r\n\r\n)
        //put this buffer in request_parser() param
        //on hold
        if (!request.empty()) {
            Request parsed_request = process_request(request); // Parse the incoming request
            this->worker.process_request(parsed_request); // Use Worker to process the request
            //if request_parser no ERROR
            // if require body, resume and fill the buffer
            this->worker.send_response(parsed_request);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling connection: " << e.what() << std::endl;
    }
    //if everything is done, we can close this session
    user_connect.close(); // Ensure that the connection is closed after handling
}
