
// #include "WebServ.hpp"


#include "Response.hpp"


void replace_string(std::string& str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Move past the replacement
    }
}


std::string readfile(std::string filename) {
    
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

std::string Response::generate_page(int status_code, std::string status_message)
{   
	std::string filename(_config.error_pages); 
    std::string html_template = readfile(filename.substr(1)); // Read the template file
    
    // Replace placeholders in the template
	std::ostringstream oss;
    oss << status_code;
    std::string code = oss.str();

    replace_string(html_template, "{{STATUS_CODE}}", code);
    replace_string(html_template, "{{ERROR_MESSAGE}}", status_message);
    replace_string(html_template, "{{METHOD}}", code);
    
    return (html_template);
}

std::string Response::generate_directory_listing(std::string directory)
{   
	std::string filename("/html_page/template/directory_listing/directory.html"); 
    std::string html_template = readfile(filename.substr(1)); // Read the template file

	std::string toInsert;
	std::string path = directory;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string file = ent->d_name;
			if (file != "." && file != "..") {
				toInsert += "<li><a href=\"" + file + "\">" + file + "</a></li>\n";
			}
		}
		closedir(dir);
	} else {
		std::cerr << "Generate Directory : Could not open directory" << directory << std::endl;
	}
    replace_string(html_template, "{{DIRECTORY}}", toInsert);    
    return (html_template);
}

Response::Response(int statusCode, const std::string& statusMessage, std::string directory)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	_config.error_pages = directory;
	set_body(generate_directory_listing(directory));
}


/*
* response header that we will send
*/
void Response::set_header(const std::string& key, const std::string& value)
{
		_headers[key] = value;
}

Response::Response()
{
	_statusCode = 200;
	_statusMessage = "OK";
}


Response::Response(int statusCode, const std::string& statusMessage, const std::string& body, Config_data c)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	_config = c;
	set_body(body);
	
}

Response::Response(int statusCode, const std::string& statusMessage, Config_data c)
{
	_config = c;
	_statusCode = statusCode;
	_statusMessage = statusMessage;

	if (_statusCode > 399) {
	set_body(generate_page(_statusCode, _statusMessage));

	}
}

Response &Response::operator=(const Response &response)
{
	_statusCode = response._statusCode;
	_statusMessage = response._statusMessage;
	_headers = response._headers;
	_body = response._body;
	return (*this);
}




/*
* response body that we will send
*/
void Response::set_body(const std::string& body)
{
	std::ostringstream oss;

	_body = body;
	oss << body.size();
	_headers["Content-Length"] = oss.str();
}

/*
* HTTP response form
EXEMPLE:

HTTP/1.1 201 Created
Content-Type: application/json
Location: http://example.com/users/123

{
  "message": "New user created",
  "user": {
	"id": 123,
	"firstName": "Example",
	"lastName": "Person",
	"email": "bsmth@example.com"
  }
}

*/
std::string Response::http_response() const
{
	std::ostringstream oss;
	std::map<std::string, std::string>::const_iterator it;

	oss <<
		"HTTP/1.1 " << this->_statusCode
		<< " " << this->_statusMessage

	<< "\r\n";    
	for (it = _headers.begin(); it != _headers.end(); ++it) {
		oss << it->first << ": " << it->second << "\r\n";
	}
	
	oss << "\r\n";
	oss << _body;
	
//	generate_error_page(this->_statusCode, this->_statusMessage);	
	return (oss.str());
}