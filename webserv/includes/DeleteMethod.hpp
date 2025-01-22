
#pragma once

#include "WebServ.hpp"
#include "IHttpMethod.hpp"

class DeleteMethod : public IHttpMethod
{
private:   
	Config_data _config;
	Request _request;
	Response _response;
	std::string _fullpath;
	std::string _route;
	int _status_code;
	bool _file_exists();
	bool _file_readable();
	bool _file_writable();
	bool _is_directory();
	int deletefile(std::string filename);
	int filesize(std::string filename);
	std::string getMimeType(const std::string& fileName);

public:
	Response handle(const Request& request, std::string& fullpath, Config_data c, std::string route);

};
