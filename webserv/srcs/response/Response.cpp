
#include "WebServ.hpp"


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

std::string Response::generate_error_page(int status_code, std::string status_message)
{   
	std::string filename(_config.error_pages); 
    std::string html_template = readfile(filename); // Read the template file
    
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
	std::string filename(_config.directory_page);	
    std::string html_template = readfile(filename); 

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

Response::Response(int statusCode, const std::string& statusMessage, std::string directory, bool isDirectory, Config_data c)
{
	_config = c;
	_header_and_body_in_one = false;
	if (isDirectory) {
		_statusCode = statusCode;
		_statusMessage = statusMessage;
		_config.error_pages = directory;
		set_body(generate_directory_listing(directory));		
	}
	else {
		_statusCode = statusCode;
		_statusMessage = statusMessage;
		set_body(generate_error_page(_statusCode, _statusMessage));		
	}	
	set_header("Content-Type", "text/html");
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
	set_body("");
	_header_and_body_in_one = false;
	//set_header("Content-Type", "text/html");
	// std::cout << ORANGE "Default Response created: " RESET << _statusCode << std::endl;
}


Response::Response(int statusCode, const std::string& statusMessage, const std::string& body, Config_data c)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	_config = c;
	set_body(body);
	_header_and_body_in_one = false;
	std::cout << ORANGE "Response created with Body, Message and Statuscode:" RESET << _statusCode << std::endl;
}

Response::Response(int statusCode, const std::string& statusMessage, const std::string& header_and_body)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	set_body(header_and_body);
	_header_and_body_in_one = true;	
	std::cout << ORANGE "Response created with header and body and Statuscode:" RESET << _statusCode << std::endl;
}

Response::Response(const std::string& header_and_body)
{	
	if (header_and_body.find("\n\n") == std::string::npos)
	{
		_statusCode = 500;
		_statusMessage = "Internal Server Error";
		set_body("Internal Server Error");
		set_header("Content-Length", "21");
		_header_and_body_in_one = false;
		return ;
	}
	std::string header = header_and_body.substr(0, header_and_body.find("\r\n\r\n"));
	std::string body = header_and_body.substr(header_and_body.find("\r\n\r\n") + 4);
	std::string status_code = header.substr(header.find("Status: ") + 8, 3);
	//std::string status_message = header.substr(header.find("Status: ") + 12, header.find("\r\n") - header.find("Status: ") - 12); // a corriger 
	std::string status_message = header.substr(header.find("Status: ") + 12, header.find("\n", header.find("Status: ") + 12) - (header.find("Status: ") + 12)); // a corriger

	//std::cout << RED "Response constructor status_message: " RESET << status_message << std::endl;

	_statusCode = std::atoi(status_code.c_str());
	_statusMessage = status_message;	
	if (_statusCode == 0)
	{
		_statusCode = 500;
		_statusMessage = "Internal Server Error";
	}
	_statusMessage = status_message;
	set_body(body);
	set_header("Content-Length", to_string(body.size()));	


	_body = header_and_body;
	//set_body(header_and_body.substr(header_and_body.find("\r\n\r\n") + 4));



	_header_and_body_in_one = true;	
	//std::cout << ORANGE "Response created with header and body." RESET << std::endl;

	//std::cout << "Response constructor header_and_body: " << header_and_body << std::endl;
	//std::cout << "Response constructor body: " << _body << std::endl;
}


Response::Response(int statusCode, const std::string& statusMessage, Config_data c)
{
	_config = c;
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	_header_and_body_in_one = false;

	if (_statusCode > 399) {
		set_body(generate_error_page(_statusCode, _statusMessage));
	}
	else {
		set_body("");
	}
	// set_header("Content-Type", "text/html");
	std::cout << ORANGE "Response created with status code: " RESET << _statusCode << std::endl;
}

Response &Response::operator=(const Response &response)
{
	_statusCode = response._statusCode;
	_statusMessage = response._statusMessage;
	_headers = response._headers;
	_body = response._body;
	_header_and_body_in_one = response._header_and_body_in_one;
	_config = response._config;

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

//	if (_header_and_body_in_one)
//	{
//		oss << _body;
//		return (oss.str());		
//	}
	oss <<
		"HTTP/1.1 " << this->_statusCode
		<< " " << this->_statusMessage

	<< "\r\n";



//	if (_header_and_body_in_one)
//	{
//		oss << _body;
//		return (oss.str());		
//	}
	for (it = _headers.begin(); it != _headers.end(); ++it) {
		oss << it->first << ": " << it->second << "\r\n";
	}
	if ( ! _header_and_body_in_one)
	{
		oss << "\r\n";
	}
//	oss << "\r\n";
	oss << _body;
	//oss << "\r\n";
	
//	generate_error_page(this->_statusCode, this->_statusMessage)

//	std::cout << "Response http_response: " << oss.str() << std::endl;
//	std::cout << "R---" << std::endl;
	return (oss.str());
}