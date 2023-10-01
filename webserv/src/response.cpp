#include "../includes/Server_handler.hpp"

void Client::configureResponseFile(std::stringstream &fileName)
{
	if (DEBUG)
		PRINT << SKY << "REQUEST TARGET: " << _requestTarget << RESET_LINE;

		std::string	exitCodeString;
		Utils::ft_itoa((int)_exitState,exitCodeString);
		std::map<std::string, string>::const_iterator it = getConfig().errorPages.find(exitCodeString);
		if (it != getConfig().errorPages.end())
		{
			fileName << "." + getConfig().root << it->second;
			return ;
		}

	if (_exitState == EXIT_OK)
	{
		if (_CGICase == PAINFULLY_TRUE )
			fileName << _cgiOutFile;
		else if (_requestTarget.compare(getConfig().root) == 0)
			fileName << STANDARD_HTML;
		else if (isDirectory(_requestTarget) && _directoryListingCase && _method.compare("GET") == 0)
		{
			directoryListing();
			return ;
		}
		else if (_method.compare("POST") == 0)
			fileName << SUCCESS_HTML;
		else
			if (!_requestTarget.empty())
				fileName << "." << _requestTarget;
	}
	else if (_method.compare("DELETE") == 0 && _exitState == NO_CONTENT)
	{
		fileName << DELETED_HTML; /* delete directories ? TEST THIS PLZ */
		_exitState = EXIT_OK;
	}
	else
	{
		fileName << "." << getConfig().root << _errorPagePath << _exitState << ".html";
	}

	/* debuggig thingies */
	if(DEBUG)
	{
		PRINT << SKY << " REQUEST TYPE: " << _method << RESET_LINE;
		PRINT << YELLOW "\tThe state: "<< _exitState << RESET_LINE;
		PRINT << YELLOW "File Name: "<< fileName.str() <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;
		PRINT << GREEN "You are trying to access '" << fileName.str() << "'" << RESET_LINE<< RESET_LINE;
	}
}

void Client::sendResponse()
{
	char      body[CHUNK_SIZE];
	std::stringstream   fileName;

	configureResponseFile(fileName);

	if (_responseState == FULLY_SENT)
		return ;
	std::ifstream inputFile((fileName.str()).c_str(), std::ios::binary);

	if (inputFile.is_open())
	{
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
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
		std::stringstream file;

		_exitState = ERROR_404;
		file << STANDARD_404;
		std::ifstream errorPage((file.str()).c_str(), std::ios::binary);
		errorPage.read(body, sizeof(body));

		_responseLength = errorPage.gcount();

		if (_responseState == INITIALIZED)
			sendHeaders();
		send(_clientFd, body, errorPage.gcount(), 0);
		errorPage.close();
		_responseState = FULLY_SENT;
		return ;
	}
}

void Client::sendHeaders()
{
 std::stringstream  headers;

 headers << HTTP_V;
 headers << " " << _exitState << " " << getHttpMsg((int)_exitState) << "\r\n";
 if (_exitState != ERROR_404 && !_CGICase)
  checkHeaders(headers);
 headers << "connection: close\r\n";
 // headers << "content-length: " << _responseLength << "\r\n";
 headers << "\r\n";
 PRINT << PURPLE << "Response:\n" << headers.str() << RESET_LINE;
 send(_clientFd, (headers.str()).c_str(), (headers.str()).length(), 0);
 _responseState = PARTIALLY_SENT;
}

void Client::directoryListing()
{
	std::stringstream       streamy;
	std::string             type;
	std::string             ref;
	struct dirent*          ent;
	DIR*                    dir = opendir(("." + _requestTarget).c_str());

	streamy << HTML_HEADERS;
	streamy << STYLES;
	streamy << BODY;
	if (dir)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") == 0)
				continue;
			type = (ent->d_type == DT_DIR)? FOLDER_FORM_OPEN : FILE_FORM_OPEN;
  			ref = (*(_path.rbegin()) == '/')? _path + ent->d_name : _path +"/"+ ent->d_name;
			streamy << type << "<a href=\"" << ref << "\">"<< std::string(ent->d_name).substr(0,20) << "</a>" <<  CLOSING_DIVS;
			if (DEBUG)
  			PRINT << ORANGE << ref << RESET_LINE;
		}
		closedir(dir);
	}
	std::string str = streamy.str();
	send(_clientFd, str.c_str(), str.size(), 0);
	_responseState = FULLY_SENT;
}

//check if the path ends with '/', if no add the '/' so the redirect link is correct