#include "../includes/Server.hpp"

void	Client::configureResponseFile(std::stringstream &fileName)
{
		if (_exitState == EXIT_OK)
	{
		if (_path.compare("/") == 0)
			fileName << STANDARD_HTML;
		else if (isDirectory(_requestTarget) && _directoryListingCase && _method.compare("GET") == 0)
		{
			directoryListing();
			_responseState = FULLY_SENT;
			return ;
		}
		else if (_method.compare("POST") == 0)
			fileName << SUCCESS_HTML;
		else
			fileName << "." << _requestTarget;
	}
	else if (_method.compare("DELETE") == 0 && _exitState == NO_CONTENT)
	{
		fileName << DELETED_HTML;	/* Can we delete directories ?? */
		_exitState = EXIT_OK;
	}
	else
		fileName << "." << _configuration.root << _errorPagePath << _exitState << ".html";

	/* debuggig thingies */
	if(DEBUG)
	{
		PRINT << SKY << " REQUEST TYPE: " << _method << RESET_LINE;
		PRINT << YELLOW "\tThe state: "<< _exitState << RESET_LINE;
		PRINT << YELLOW "File Name: "<< fileName.str() <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;
		PRINT << GREEN "You are trying to access '" << fileName.str() << "'" << RESET_LINE<< RESET_LINE;
	}
}

void	Client::sendResponse()
{
	char						body[CHUNK_SIZE];
	std::stringstream			fileName;

	configureResponseFile(fileName);

	std::ifstream	inputFile((fileName.str()).c_str(), std::ios::binary);
	
	if (inputFile.is_open())
	{
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
		// _responseLength = inputFile.gcount();
		if (_responseState == INITIALIZED)
			sendHeaders();
		send(_clientFd, body, inputFile.gcount(), 0);

		if (inputFile.eof())
		{
			PRINT << YELLOW "\t END OF FILE BTW" << RESET_LINE; 
			_responseState = FULLY_SENT;
			inputFile.close();
			return ;
		}
		_responsePos = inputFile.tellg();
		inputFile.close();
	}
	else if (!inputFile) 
	{
		std::stringstream	file;

		_exitState = ERROR_404;
		file << STANDARD_404;
		std::ifstream errorPage((file.str()).c_str(), std::ios::binary); /* add protection */
		errorPage.read(body, sizeof(body));
		// _responseLength = errorPage.gcount();
		if (_responseState == INITIALIZED)
			sendHeaders();
		send(_clientFd, body, errorPage.gcount(), 0);
		errorPage.close();
		_responseState = FULLY_SENT;
		return ;
	}
}

void	Client::sendHeaders()
{
	std::stringstream		headers;

	headers << HTTP_V;
	headers << " " << _exitState << " " << getHttpMsg((int)_exitState) << "\r\n";
	if (_exitState != ERROR_404)
		checkHeaders(headers);
	headers << "connection: keep-alive\r\n";
	// headers << "content-length: " << _responseLength << "\r\n";
	headers << "\r\n";
	PRINT << PURPLE << "Response:\n" << headers.str() << RESET_LINE;
	send(_clientFd, (headers.str()).c_str(), (headers.str()).length(), 0);
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
	send(_clientFd, str.c_str(), str.size(), 0);
	_responseState = FULLY_SENT;
}