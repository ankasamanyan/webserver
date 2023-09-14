#include "../includes/Server.hpp"

void	Client::sendResponse()
{
	char			body[CHUNK_SIZE];
	string			fileName;
	string			str;

	Utils::ft_itoa(((int)_exitState), str);
	if (_exitState == EXIT_OK)
	{
		if (_path.compare("/") == 0)
			fileName = STANDARD_HTML;
		else if (isDirectory(_requestTarget) && _directoryListingCase && _method.compare("GET") == 0)
		{
			directoryListing();
			return ;
		}
		else if (_method.compare("POST") == 0)
			fileName = SUCCESS_HTML;
		else
			fileName = "." + _requestTarget;
	}
	else if (_method.compare("DELETE") == 0 && _exitState == NO_CONTENT)
	{
		fileName = DELETED_HTML;	/* Can we delete directories ?? */
		_exitState = EXIT_OK;
	}
	else
	{
		fileName.append(".").append(_configuration.root).append(_errorPagePath);
		fileName.append(str).append(".html");
	}
	PRINT << SKY << " REQUEST TYPE: " << _method << RESET_LINE;
	PRINT << PINK "The code: "<< "\t" << str << YELLOW "\tThe state: "<< _exitState << RESET_LINE;
	PRINT << YELLOW "File Name: "<< fileName <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;

	PRINT << GREEN "You are trying to access '" << fileName << "'" << RESET_LINE<< RESET_LINE;

	std::ifstream	inputFile(fileName.c_str(), std::ios::binary); /* temp html file */
	
	if (inputFile.is_open())
	{
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
		if (_responseState == INITIALIZED)
			sendHeaders();
		send(_clientFd, body, inputFile.gcount(), 0);

		if (inputFile.eof())
		{
			PRINT << YELLOW "\t END OF FOLE" << RESET_LINE; 
			_responseState = FULLY_SENT;
			inputFile.close();
			return ;
		}
		_responsePos = inputFile.tellg();
		inputFile.close();
	}
	else if (!inputFile) 
	{
		_exitState = ERROR_404;
		if (_responseState == INITIALIZED)
			sendHeaders();
		fileName = STANDARD_404;
		std::ifstream errorPage(fileName.c_str(), std::ios::binary);
		errorPage.read(body, sizeof(body));
		send(_clientFd, body, errorPage.gcount(), 0);
		errorPage.close();
		_responseState = FULLY_SENT;
		return ;
		/*  */
		std::cout << PINK << "HALP, THERE IS PROBLEM WITH THE FILE " << RESET_LINE;
		std::cout << PINK << strerror(errno) << RESET_LINE;
	}
}

void	Client::sendHeaders()
{
	string			headers;
	string			response;
	string			errorCode;
	string			str;

	Utils::ft_itoa((int)_exitState, str);
	response.append(HTTP_V);
	errorCode.append(" ").append(str).append(" ").append(getHttpMsg((int)_exitState));
	response.append(errorCode).append("\r\n");
	PRINT << PURPLE << "ERROR CODES:\t" << str << RESET_LINE;
	if (_exitState != ERROR_404)
		checkHeaders(headers);
	headers.append("\r\n");
	response.append(headers);
	PRINT << PURPLE << "Response:\n" << response << RESET_LINE;
	send(_clientFd, response.c_str(), response.length(), 0);
	_responseState = PARTIALLY_SENT;
}

void	Client::directoryListing()
{
	std::stringstream	streamy;
	struct dirent*		ent;
	DIR* 				dir = opendir(("." + _requestTarget).c_str());

	streamy << "HTTP/1.1 200 OK\r\n\r\n";
	//read the html file
	streamy << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
	streamy	<< "<head><title>Test Website for 42 Project: webserv</title><link rel=\"stylesheet\" type=\"text/css\"/></head>"
		<< "<html><body><h1>Directory Listing</h1><ul>";
	if (dir)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") == 0)
				continue;
			if (ent->d_type == DT_DIR) // append a slash if it's a directory
				streamy << "<li><a href=\"" << _path + ent->d_name << "/\">" << ent->d_name << "/</a></li>";
			else
				streamy << "<li><a href=\"" << _path + ent->d_name << "\">" << ent->d_name << "</a></li>";
			
		}
		closedir(dir);
	}
	streamy << "</ul></body></html>";
	std::string str = streamy.str();
	send(_clientFd, str.c_str() ,str.size(), 0);
	_responseState = FULLY_SENT;
}