
#include "WebServ.hpp"

bool GetMethod::_file_exists()
{
    struct stat buffer;
    return (stat (_fullpath.c_str(), &buffer) == 0);
}

bool GetMethod::_file_readable()
{
    if (access(_fullpath.c_str(), R_OK) == 0) {
        return true;
    }
    return false;
}

bool GetMethod::_file_writable()
{
        if (access(_fullpath.c_str(), W_OK) == 0) {
            return true;
        }
        return false;
}

bool GetMethod::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}

int GetMethod::filesize(std::string filename) {
    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) == 0) {
        return fileStat.st_size;
    } else {
        return -1;
    }
}

std::string GetMethod::getMimeType(const std::string& fileName) {
    // Map of file extensions to MIME types
    std::map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".txt", "text/plain"},
        {".jpg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".json", "application/json"},
        {".xml", "application/xml"},
        {".pdf", "application/pdf"},
        // Add more as needed
    };

    // Find the file extension
    size_t dotPos = fileName.rfind('.');
    if (dotPos != std::string::npos) {
        std::string extension = fileName.substr(dotPos);
        if (mimeTypes.count(extension)) {
            return mimeTypes[extension];
        }
    }

    return "application/octet-stream"; // Default binary type
}

std::string GetMethod::readfile(std::string filename) {
    
    std::ifstream file(filename); // Open the file in read mode
    if (!file) {
        std::cerr << "Error: Could not open:" << filename << std::endl;        
    }

    std::string content;
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {
        content += line + "\n"; // Append each line to the content string
    }

    file.close(); // Close the file
    return content;    
}


Response GetMethod::handle(const Request& request, std::string& fullpath)
{
    // Handle GET request
    // Read file or generate content
    // Return Response object
    /* EXEMPLE */
    
    _request = request;
    _fullpath = fullpath;


    Response response;
    
    // Process the request
    if (filesize(_fullpath) == -1) {
        response = Response(404, "Not Found");
        return response;
    }
    else if (filesize(_fullpath) > MAX_FILE_SIZE) {
        response = Response(413, "Request Entity Too Large");
        return response;
    }
    else if (! _file_readable()) {
        response = Response(403, "Forbidden");
        return response;
    }
//    else if (_is_directory()) {
//        response = Response(403, "Forbidden");
//        return response;
//    }
    else {
        std::string content = readfile(_fullpath);                
        response.set_body(content);
        response.set_header("Content-Type", getMimeType(_fullpath));
    }    
    return (response);
}
