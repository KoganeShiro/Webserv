
#include "WebServ.hpp"

/*
* response header that we will send
*/
void Response::set_header(const std::string& key, const std::string& value)
{
		_headers[key] = value;
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
std::string http_response() const
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
	
	generate_error_page(this->_statusCode, this->_statusMessage);
	if (!_body.empty()) {
		oss.write(&_body[0], _body.size());
	}
	return (oss.str());
}