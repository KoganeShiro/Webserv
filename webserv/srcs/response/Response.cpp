
#include "WebServ.hpp"

void replace_string(std::string& str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Move past the replacement
    }
}


std::string readfile(std::string filename) {
    
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

std::string generate_page(int status_code, std::string status_message)
{    
    std::string html_template = readfile("html_page/template/error/index.html");
    
    // Replace placeholders in the template

    replace_string(html_template, "{{STATUS_CODE}}", std::to_string(status_code));
    replace_string(html_template, "{{ERROR_MESSAGE}}", status_message);
    replace_string(html_template, "{{METHOD}}", std::to_string(status_code));
    
    return (html_template);
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


Response::Response(int statusCode, const std::string& statusMessage, const std::string& body)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
	set_body(body);
}


Response::Response(int statusCode, const std::string& statusMessage)
{
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