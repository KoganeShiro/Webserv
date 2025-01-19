
#include "PostMethod.hpp"

bool PostMethod::_file_exists()
{
    struct stat buffer;
    return (stat (_fullpath.c_str(), &buffer) == 0);
}

bool PostMethod::_file_readable()
{
    if (access(_fullpath.c_str(), R_OK) == 0) {
        return (true);
    }
    return (false);
}

bool PostMethod::_file_writable()
{
        if (access(_fullpath.c_str(), W_OK) == 0) {
            return (true);
        }
        return (false);
}

bool PostMethod::_is_directory()
{
    struct stat buffer;
    if (stat (_fullpath.c_str(), &buffer) == 0) {
        return (S_ISDIR(buffer.st_mode));
    }
    return (false);
}

int PostMethod::filesize(std::string filename)
{
    struct stat fileStat;
    std::cout << "PostMethod Filesize check: " << filename << std::endl;
    if (stat(filename.c_str(), &fileStat) == 0) {
        return (fileStat.st_size);
    } else {
        return (-1);
    }
}

std::string PostMethod::getMimeType(const std::string& fileName)
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

/*
std::string PostMethod::readfile(std::string filename) {
    
    std::ifstream file(filename.c_str()); // Open the file in read mode
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
*/

/*
filename=create_new_file&content=hello+world+%21
*/

static std::string _urlDecode(const std::string &encoded)
{
    std::ostringstream decoded;

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.length()) {
                std::istringstream hex(encoded.substr(i + 1, 2));
                int value;
                hex >> std::hex >> value;
                decoded << static_cast<char>(value);
                i += 2;
            }
        } else if (encoded[i] == '+') {
            decoded << ' ';
        } else {
            decoded << encoded[i];
        }
    }
    return (decoded.str());
}

void PostMethod::_parse_body(std::string &content, std::string &filename)
{
    if (content.find("filename=") != std::string::npos 
            && content.find("&content=") != std::string::npos) {
        filename += "/";
        filename += content.substr(content.find("filename=") + 9, content.find("&content=") - (content.find("filename=") + 9));
        content = content.substr(content.find("&content=") + 9);
        content = _urlDecode(content);
    }
    std::cout << "PostMethod Parse filename: " << filename << std::endl;
    std::cout << "PostMethod Parse content: " << content << std::endl;
}   

void PostMethod::_extract_next_part()
{
    next_part = "";
    size_t start_pos = _request.get_body().find(_multipath_boundary);
    if (start_pos != std::string::npos) {
        start_pos += _multipath_boundary.length() + 2; // Move past boundary and CRLF
        std::cout << MAGENTA "PostMethod Start pos: " << start_pos << std::endl;
        size_t end_pos = _request.get_body().find(_multipath_boundary, start_pos);
        std::cout << "PostMethod End pos: " << end_pos << std::endl;
        if (end_pos != std::string::npos) {
            next_part = _request.get_body().substr(start_pos, end_pos - start_pos - 2); // Exclude trailing CRLF
            std::cout << "PostMethod Next part: " << next_part << RESET << std::endl;
            _request.set_body(_request.get_body().substr(end_pos));
        }
    }
}

void PostMethod::_extract_content()
{
    size_t pos = next_part.find("filename=\"");
    if (pos != std::string::npos) {
        pos += 10;
        size_t end_pos = next_part.find("\"", pos);        
        if (end_pos != std::string::npos) {
            _filename = next_part.substr(pos, end_pos - pos);
            pos = next_part.find("\r\n\r\n") + 4;
            _content = next_part.substr(pos);
        }
    }
}

void PostMethod::_set_filename_and_content()    
{
    _filename = "";
    _content = "";        
    _extract_next_part();
    while (next_part != "" && _filename == "")
    {
        _extract_content();
        if (_filename != "")
        {  
            _full_filename = _fullpath + _filename;
        }
        _extract_next_part();
    }
}

void PostMethod::_set_multipath_boundary()
{
    // Extract the boundary from the content type OK
    this->_content_type = this->_request.get_header_element("Content-Type");
    std::string boundary_prefix = "boundary=";
    size_t pos = this->_content_type.find(boundary_prefix);
    if (pos != std::string::npos) {
        _multipath_boundary = _content_type.substr(pos + boundary_prefix.length());
    }
    std::cout << BLUE "PostMethod Multipath boundary: {" << _multipath_boundary << "}" << std::endl;
}


// void PostMethod::_set_multipath_boundary()
// {
//     // Extract the boundary from the content type OK
//     size_t pos = _content_type.find("boundary=");
//     if (pos != std::string::npos) {
//         _multipath_boundary = _content_type.substr(pos + 9);
//     }
//     std::cout << BLUE "PostMethod Multipath boundary: " << _multipath_boundary << std::endl;
// }

void PostMethod::_upload_file(std::string &content, std::string &filename)
{
    //std::string boundary = extract_boundary(_request.get_header_element("Content-Type"));
    _set_multipath_boundary();
    _fullpath = _config.routes[_route].root_dir + "/";
    _set_filename_and_content();
    std::cout << "PostMethod Upload filename: " << _filename << std::endl;
    
    //content = clean_request_body(content);
    content = this->_content;
    filename = this->_full_filename;
}

int PostMethod::writefile(std::string filename, std::string content)
{
    //call CGI script
        //execve
    if (_file_exists() && ! _file_writable()) {
        std::cerr << "PostMethod Error: File exists and is not writable:" << filename << std::endl;
        return (-1);
    }
    if (content.empty()) {
        std::cerr << "PostMethod Error: Empty body" << std::endl;
        return (-1);
    }
    // std::cout << MAGENTA "PostMethod Writefile: [" << filename << "]" << std::endl;
    // std::cout << "Content: [" << content << "]" RESET << std::endl;
    if (this->_request.get_header_element("Content-Type") == "application/x-www-form-urlencoded") {
        _parse_body(content, filename);
    } 
    else {
        _upload_file(content, filename);
    }
    // Create file
    if (filename.empty()) {
        return (NO_FILENAME);
    }
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "PostMethod Error: Could not create the file to write in!" << std::endl;
        return (ERROR);
    }
    file << content;
    file.close();
    return (0);
}

Response PostMethod::handle(const Request& request, std::string& fullpath, Config_data c, std::string route)
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
    if (_request.get_header_element("Content-Length") == "") {
        response = Response(411, "Length Required", _config);
        std::cout << "PostMethod: Content-Length header missing" << std::endl;
        return (response);
    }
    else if (_request.get_header_element("Content-Type") == "") {
        response = Response(400, "Bad Request", _config);
        std::cout << "PostMethod: Content-Type header missing" << std::endl;
        return (response);
    }
    else if (std::atoi((_request.get_header_element("Content-Length")).c_str()) > MAX_FILE_SIZE) {
        response = Response(413, "Payload Too Large", _config);
        std::cout << "PostMethod: Payload Too Large" << std::endl;
        return (response);
    }
    /*
    if (filesize(_fullpath) == -1) {
        response = Response(404, "Not Found", _config);
        std::cout << "File not found: " << _fullpath << std::endl;
        return response;
    }
    else if (filesize(_fullpath) > MAX_FILE_SIZE) {
        response = Response(413, "Request Entity Too Large", _config);
        std::cout << "File too large: " << _fullpath << std::endl;
        return response;
    }
    else if (! _file_readable()) {
        response = Response(403, "Forbidden", _config);
        std::cout << "File not readable: " << _fullpath << std::endl;
        return response;
    }
    else if (_is_directory() && ! _config.routes[_route].dir_listing) {
        response = Response(403, "Forbidden", _config);
        std::cout << "Directory listing not allowed: " << _fullpath << std::endl;
        return response;
    }
    else if (_is_directory() && _config.routes[_route].dir_listing) {
        response = Response(200, "OK", _fullpath, true);
        std::cout << "Directory listing OK:" << _fullpath << std::endl;
        return response;
    }
    */
    // else if (checkRoute() == "cgi") {
    //     response = Response(501, "Not Implemented", _config);
    //     std::cout << "CGI not implemented: " << _fullpath << std::endl;
    //     return response;
    // }

    else {
        int res = writefile(_fullpath, _request.get_body());
        if (res == ERROR) {
            response = Response(500, "Internal Server Error. Could not create file", _config);
            std::cout << "Error writing file: " << _fullpath << std::endl;
            return (response);
        }
        else if (res == NO_FILENAME) {
            response = Response(400, "Bad Request", _config);
            std::cout << "PostMethod: Filename not found" << std::endl;
            return (response);
        }
        response = Response(201, "Created", _config);    
        response.set_header("Location", _request.get_path());
   //     response.set_header("Content-Type", getMimeType(_fullpath));
    }
    return (response);
}
