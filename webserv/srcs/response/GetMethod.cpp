
// #include "WebServ.hpp"
#include "GetMethod.hpp"

bool GetMethod::_file_exists()
{
    struct stat buffer;

    return (stat (_fullpath.c_str(), &buffer) == 0);
}

bool GetMethod::_file_readable()
{
    if (access(_fullpath.c_str(), R_OK) == 0) {
        return (true);
    }
    return (false);
}

bool GetMethod::_file_writable()
{
        if (access(_fullpath.c_str(), W_OK) == 0) {
            return (true);
        }
        return (false);
}

bool GetMethod::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return (S_ISDIR(buffer.st_mode));
    }
    return (false);
}

int GetMethod::filesize(std::string filename)
{
    struct stat fileStat;
    std::cout << "Get Method Filesize check: " << filename << std::endl;
    if (stat(filename.c_str(), &fileStat) == 0) {
        return (fileStat.st_size);
    } else {
        return (-1);
    }
}

std::string GetMethod::getMimeType(const std::string& fileName) {
    // Map of file extensions to MIME types
    std::map<std::string, std::string> mime;
    mime.insert(std::make_pair(".html", "text/html"));
    mime.insert(std::make_pair(".txt", "text/plain"));
    mime.insert(std::make_pair(".jpg", "image/jpeg"));
    mime.insert(std::make_pair(".png", "image/png"));
    mime.insert(std::make_pair(".gif", "image/gif"));
    mime.insert(std::make_pair(".json", "application/json"));
    mime.insert(std::make_pair(".xml", "application/xml"));
    mime.insert(std::make_pair(".pdf", "application/pdf"));

    // Find the file extension
    size_t dotPos = fileName.rfind('.');
    if (dotPos != std::string::npos) {
        std::string extension = fileName.substr(dotPos);
        if (mime.count(extension)) {
            return (mime[extension]);
        }
    }

    return ("application/octet-stream"); // Default binary type
}

std::string GetMethod::readfile(std::string filename) {
    
    std::ifstream file(filename.c_str()); // Open the file in read mode
    if (!file) {
        std::cerr << "Get Method Error: Could not open:" << filename << std::endl;        
    }

    std::string content;
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {
        content += line + "\n"; // Append each line to the content string
    }

    file.close(); // Close the file
    return (content);    
}


Response GetMethod::handle(const Request& request, std::string& fullpath, Config_data c, std::string route)
{
    // Handle GET request
    // Read file or generate content
    // Return Response object
    /* EXEMPLE */
    
    _request = request;
    _fullpath = fullpath;
    _config = c;
    _route = route;

    Response response;
    
    // Process the request
    if (_is_directory() && ! _config.routes[_route].dir_listing) {
        response = Response(403, "Forbidden", _config);
        response.set_header("Content-Type", "text/html");
        std::cout << "Get Method Directory listing not allowed: " << _fullpath << std::endl;
        return (response);
    }
    else if (_is_directory() && _config.routes[_route].dir_listing) {
        response = Response(200, "OK", _fullpath, true, _config);
        std::cout << "Get Method Directory listing OK:" << _fullpath << std::endl;
        return (response);
    }

    else if (filesize(_fullpath) == -1) {
        response = Response(404, "Not Found", _config);
        response.set_header("Content-Type", "text/html");
        std::cout << "Get Method File not found: " << _fullpath << std::endl;
        return (response);
    }
    else if (filesize(_fullpath) > MAX_FILE_SIZE) {
        response = Response(413, "Request Entity Too Large", _config);
        response.set_header("Content-Type", "text/html");
        std::cout << "Get Method File too large: " << _fullpath << std::endl;
        return (response);
    }
    else if (! _file_readable()) {
        response = Response(403, "Forbidden", _config);
        response.set_header("Content-Type", "text/html");
        std::cout << "Get Method File not readable: " << _fullpath << std::endl;
        return (response);
    }
//    else if (_is_directory() && ! _config.routes[_route].dir_listing) {
 //       response = Response(403, "Forbidden", _config);
  //      std::cout << "Get Method Directory listing not allowed: " << _fullpath << std::endl;
 //       return response;
  //  }
  //  else if (_is_directory() && _config.routes[_route].dir_listing) {
  //      response = Response(200, "OK", _fullpath, true, _config);
  //      std::cout << "Get Method Directory listing OK:" << _fullpath << std::endl;
  //      return response;
  //  }
    else {
        std::string content = readfile(_fullpath);                        
        response.set_body(content);
        response.set_header("Content-Type", getMimeType(_fullpath));
    }    
    return (response);
}
