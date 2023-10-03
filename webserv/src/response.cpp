#include "../includes/Server_handler.hpp"

void Client::configureResponseFile(std::stringstream &fileName)
{
	if (customErrorPageExists(fileName))
		return ;

	if (_exitState == EXIT_OK)
	{
		successfulRequest(fileName);
		if (_responseState == FULLY_SENT)
			return ;
	}
	else if (_method.compare("POST") == 0 && _exitState == CREATED)
	{
		sendHeadersAndQuit();
	}
	else if (_method.compare("DELETE") == 0 && _exitState == NO_CONTENT)
	{
		sendHeadersAndQuit();
	}
	else
		fileName << "." << getConfig().root << _errorPagePath << _exitState << ".html";

	/* debuggig thingies */
	if(DEBUG)
	{
		PRINT << SKY << " REQUEST TYPE: " << _method << RESET_LINE;
		PRINT << YELLOW "\tThe state: "<< _exitState << RESET_LINE;
		PRINT << YELLOW "File Name: "<< fileName.str() <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;
		PRINT << GREEN "You are trying to access '" << fileName.str() << "'" << RESET_LINE<< RESET_LINE;
	}
}

void	Client::successfulRequest(std::stringstream &fileName)
{
	if (_CGICase == PAINFULLY_TRUE)
	{
		fileName << _cgiOutFile;
	}
	else if (_requestTarget.compare(getConfig().root) == 0)
	{
		fileName << STANDARD_HTML;
	}
	else if (isDirectory(_requestTarget) && _directoryListingCase 
			&& _method.compare("GET") == 0)
	{
		directoryListing();
		return ;
	}
	else
	{
		if (!_requestTarget.empty())
			fileName << "." << _requestTarget;
	}
}

bool	Client::customErrorPageExists(std::stringstream &fileName)
{
	std::string	exitCodeString;

	Utils::ft_itoa((int)_exitState,exitCodeString);
	std::map<std::string, string>::const_iterator it = getConfig().errorPages.find(exitCodeString);
	if (it != getConfig().errorPages.end())
	{
		fileName << "." + getConfig().root << it->second;
		return true;
	}
	else
		return false;
}

void	Client::sendHeadersAndQuit()
{
	sendHeaders();
	_responseState = FULLY_SENT;
	return ;
}

void	Client::sendResponse()
{
	std::stringstream   fileName;

	configureResponseFile(fileName);
	if (_responseState == FULLY_SENT)
		return ;
	_responseLength = fileSize(fileName.str());

	std::ifstream inputFile((fileName.str()).c_str(), std::ios::binary);

	if (inputFile.is_open())
	{
		readTheFile(inputFile);
	}
	else if (!inputFile) 
	{
		noInputFile();
		return ;
	}
}

void	Client::noInputFile()
{
	char      body[CHUNK_SIZE];
	std::stringstream file;

	_exitState = ERROR_404;
	file << STANDARD_404;
	std::ifstream errorPage((file.str()).c_str(), std::ios::binary);
	errorPage.read(body, sizeof(body));
	_responseLength = fileSize(file.str());
	if (_responseState == INITIALIZED)
		sendHeaders();
	send(_clientFd, body, errorPage.gcount(), 0);
	errorPage.close();
	_responseState = FULLY_SENT;
}

void	Client::readTheFile(std::ifstream   &inputFile)
{
	char      body[CHUNK_SIZE];

	inputFile.seekg(_responsePos);
	inputFile.read(body, sizeof(body));
	if (_responseState == INITIALIZED)
		sendHeaders();
	size_t sendResultCount = send(_clientFd, body, inputFile.gcount(), 0);
	if (sendResultCount <= 0)
	{
		_clientState = SHOULD_DISCONNECT_;
		return ;
	}
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

void	Client::sendHeaders()
{
	std::stringstream  headers;

	headers << HTTP_V;
	headers << " " << _exitState << " " << getHttpMsg((int)_exitState) << "\r\n";
	if (_exitState != ERROR_404 && !_CGICase)
		checkHeaders(headers);
	headers << "connection: close\r\n";
	if (_responseLength)
		headers << "content-length: " << _responseLength << "\r\n";
	headers << "\r\n";
	PRINT << PURPLE << "Response:\n" << headers.str() << RESET_LINE;
	size_t	sendResultCount = send(_clientFd, (headers.str()).c_str(), (headers.str()).length(), 0);
	if (sendResultCount <= 0)
		_clientState = SHOULD_DISCONNECT_;
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
	size_t sendResultCount = send(_clientFd, str.c_str(), str.size(), 0);
	if (sendResultCount <= 0)
		_clientState = SHOULD_DISCONNECT_;
	_responseState = FULLY_SENT;
}

size_t Client::fileSize(const std::string &filePath)
{
	struct stat	fileInfo;

	if (filePath.empty())
		return 0;
	if (stat(filePath.c_str(), &fileInfo) != 0)
		return 0;
	return static_cast<size_t>(fileInfo.st_size);
}
