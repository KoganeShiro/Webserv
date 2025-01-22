
#include "DeleteMethod.hpp"

bool DeleteMethod::_file_exists()
{
    struct stat buffer;
    return (stat (_fullpath.c_str(), &buffer) == 0);
}

bool DeleteMethod::_file_readable()
{
    if (access(_fullpath.c_str(), R_OK) == 0) {
        return (true);
    }
    return (false);
}

bool DeleteMethod::_file_writable()
{
        if (access(_fullpath.c_str(), W_OK) == 0) {
            return (true);
        }
        return (false);
}

bool DeleteMethod::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return (S_ISDIR(buffer.st_mode));
    }
    return (false);
}

int DeleteMethod::filesize(std::string filename)
{
    struct stat fileStat;
    std::cout << "DeleteMethod Filesize check: " << filename << std::endl;
    if (stat(filename.c_str(), &fileStat) == 0) {
        return (fileStat.st_size);
    } else {
        return (-1);
    }
}

std::string DeleteMethod::getMimeType(const std::string& fileName)
{
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

int DeleteMethod::deletefile(std::string filename)
{
    if (remove(filename.c_str()) != 0) {
        std::cerr << "DeleteMethod Error: Could not delete file:" << filename << std::endl;
        return (-1);
    }
    std::cout << GREEN "FILE :" << filename << " HAS BEEN REMOVED" << std::endl;
    return (0);
}

Response DeleteMethod::handle(const Request& request, std::string& fullpath, Config_data c, std::string route)
{
    _request = request;
    _fullpath = fullpath;
    _config = c;
    _route = route;

    Response response;
    std::cout << "fullpath" << _fullpath << std::endl;
    if (deletefile(_fullpath) == -1) {
        response = Response(500, "Internal Server Error. Could not delete file", _config);
        std::cout << "DeleteMethod Error writing file: " << _fullpath << std::endl;
        return (response);
    }
    response = Response(204, "No Content", _config); 
    return (response);
}
