#include "../includes/Server.hpp"

void	Client::receiveRequest() 
{
    char currentChunk[CHUNK_SIZE];

    memset(currentChunk, 0, CHUNK_SIZE);
    ssize_t numberOfBytesReceived = recv(_clientFd, currentChunk, CHUNK_SIZE, 0);
	if (numberOfBytesReceived == 0)
	{
		_clientState = SHOULD_DISCONNECT_;
		return ; 
	}
    if (numberOfBytesReceived < 0) {
        Utils::printMsg("Error receiving a message from a socket", PURPLE);
    }
    PRINT << SKY "The REQUEST" << RESET_LINE;
    // PRINT << currentChunk << RESET_LINE;
  	_request.append(currentChunk, numberOfBytesReceived);
    if (numberOfBytesReceived < CHUNK_SIZE) 
	{
        if (isRequestValid() == false) {
			_clientState = INVALID_;
			return ;
        }
        if (_CGICase == true) {
        //Юля, твой выход😎
        }
        prepareResponse();
		_clientState = VALID_;
        return ;
    }
	_clientState = PARTLY_READ_;
    return ;
}


bool    Client::isRequestValid() {
    if (isRequestEmpty() == true)
        return false;
    parseRequest();
    updateDirectoryIfUploading();
    setDefaultFile();
    defineRequestTarget();
    assignCGIFlag();
    return areAllPartsOfRequestValid();
}

bool    Client::isRequestEmpty() {
    if (_request.empty()) {
        _exitState = BAD_REQUEST;
        return true;
    }
    return false;
}

void    Client::parseRequest() {
    parseRequestLine();
    parseHeaders();
    parseBody();
}

void    Client::parseRequestLine() {
    std::string	requestLine = _request.substr(0, _request.find("\r\n"));
    std::vector<string> splitRequestLine = Utils::split(requestLine, ' ');
    _method = splitRequestLine[0];
    _path = splitRequestLine[1];
    _HTTPVersion = splitRequestLine[2];
    _directory = getDirectory();
    _file = getFile();
}

std::string Client::getDirectory() {
    std:::string pathWithRoot = configuration.root + _path;
    if (isDirectory(pathWithRoot) && _path[_path.size() - 1] != '/')
        _path.append("/");
    return _path.substr(0, _path.find_last_of("/") + 1);
}

std::string Client::getFile() {
    std::string file;
    if (_path.find_last_of("/") != string::npos)
        file = _path.substr(_path.find_last_of("/") + 1);
    else
        file = "";
    return file;
}

void    Client::parseHeaders() {
    int startHeadersIndex = _request.find('\n') + 1;
    int headersSize = _request.find("\r\n\r\n") - startHeadersIndex;
    std::string header = _request.substr(startHeadersIndex, headersSize);
    std::vector<string> splitHeaders = Utils::split(header, '\n');
    for (size_t i = 0; i < splitHeaders.size(); i++) {
        std::string trimmedHeader = Utils::trimRight(splitHeaders[i], "\r\n");
        std::vector<string> headerFieldValue = Utils::split(trimmedHeader, ':');
        headerFieldValue[1] = Utils::trimLeft(headerFieldValue[1], " ");
        _headers.insert(std::make_pair(headerFieldValue[0], headerFieldValue[1]));
    }
}

void    Client::parseBody() {
    if (_method != "POST")
        return ;
    size_t startBodyIndex = _request.find("\r\n\r\n") + 4;
    size_t bodySize;
    if (_headers.find("Content-Length") != _headers.end())
        bodySize = atoi(_headers.at("Content-Length").c_str());
    else
        bodySize = string::npos;
    _body = _request.substr(startBodyIndex, bodySize);
}

void    Client::updateDirectoryIfUploading() {
    std::map<std::string, location>::const_iterator it = _configuration.locations.find(_directory);

	if (it != _configuration.locations.end() && _method == POST && !it->second.uploadDirectory.empty())
		_directory = it->second.uploadDirectory;
}

void    Client::setDefaultFile() {
    std::map<std::string, location>::const_iterator it = _configuration.locations.find(_directory);

    if (it != _configuration.locations.end() && !it->second.defaultFile.empty())
        _defaultFile = _configuration.root + it->second.defaultFile;
    else
        _defaultFile = "/html/errorHtml/404.html";
}

void    Client::defineRequestTarget() {
    _directoryListingCase = false;
    _requestTarget = _configuration.root + _directory + _file;
    redirectIfNeeded();
    if (isDirectory(_requestTarget))
        assignContent();
}

void    Client::redirectIfNeeded() {
    std::map<std::string, location>::const_iterator it = _configuration.locations.find(_directory);

    if (it != _configuration.locations.end() && !it->second.redirectionDirectory.empty()) {
        _requestTarget = _configuration.root + it->second.redirectionDirectory + _file;
    }
}

bool    Client::isDirectory(std::string path) {
    struct stat buffer;
    std::string filePath = "." + path;
    if (stat(filePath.c_str(), &buffer) == 0)
        return S_ISDIR(buffer.st_mode);
    return false;
}

void    Client::assignContent() {
    if (_configuration.dirListing == false)
        _requestTarget = _defaultFile;
    else
        _directoryListingCase = true;
}

void    Client::assignCGIFlag() {
    int CGIDirectoryLength = _configuration.CGIDir.length();
    std::string partOfPath = _path.substr(0, CGIDirectoryLength);
    if (_configuration.CGIDir == partOfPath)
        _CGICase = true;
    else
        _CGICase = false;
}

bool    Client::areAllPartsOfRequestValid() {
    if (isPathAllowed() == false ||
        isMethodAllowed() == false ||
        isHTTPVersionValid() == false ||
        isContentOfAllowedSize() == false)
        return false;
    return true;
}

bool Client::isPathAllowed {
    std::map<std::string, location>::const_iterator it = _configuration.locations.find(_directory);

    if (it == _configuration.locations.end()) {
        assignErrorForInvalidPath();
        return false;
    }
    return true;
}

void    Client::assignErrorForInvalidPath() {
    std::string fileToCheck = "." + _configuration.root + _directory;
    if (exists(fileToCheck))
        _exitState = FORBIDDEN;
    else
        _exitState = ERROR_404;
}

bool    Client::isMethodAllowed() {
    std::map<std::string, location>::const_iterator it = _configuration.locations.find(_directory);
	_location = it->second;

    if ((_method == "GET" && _location.methodGet) ||
        (_method == "DELETE" && _location.methodDelete) ||
        (_method == "POST" && _location.methodPost)
        return true;
    _exitState = METHOD_NOT_ALLOWED;
    return false;
}

bool    Client::isHTTPVersionValid() {
    if (_HTTPVersion == "HTTP/1.1")
        return true;
    _exitState = INVALID_HHTPV;
    return false;
}

bool    Client::isContentOfAllowedSize() {
    int allowedSize = atoi(_configuration.maxBody.c_str());

    if (_headers.find("Content-Length") != _headers.end()) {
        int contentSize = atoi(_headers.at("Content-Length").c_str());
        if (contentSize <= allowedSize)
            return true;
    }
    else {
        int actualBodySize = _body.length();
        if (actualBodySize <= allowedSize)
            return true;
    }
    _exitState = CONTENT_TOO_LARGE;
    return false;
}

clientState Client::getState() {
	return _clientState;
}

void    Client::prepareResponse() {
    if (_method == "GET")
        handleGet();
    else if (_method == "POST")
        handlePost();
    else if (_method == "DELETE")
        handleDelete();
}

void    Client::handleGet() {

}

void    Client::handlePost() {
    if (isInsideUploads() == false)
        _exitState = FORBIDDEN;
    else {
        std::string pathToPost = "." + _configuration.locations["root"].locationDir + _path;
        std::ofstream outputFileStream;
        outputFileStream.open((pathToPost).c_str(), std::ios::binary | std::ios::app);
        outputFileStream.write(_body.c_str(), _body.length());
        outputFileStream.close();
    }
}

void    Client::handleDelete() {
    std::string fileToDelete = "." + _configuration.root + _path;

    if (isAllowedToDelete() == false)
        _exitState = FORBIDDEN;
    else if (exists(fileToDelete) == false)
        _exitState = ERROR_404;
    else
        attemptToRemove(fileToDelete);
}

bool    Client::isAllowedToDelete() {
    return !(isDirectory(_configuration.root + _path) || isInsideUploads() == false);
}

bool    Client::isInsideUploads() {
    if (_path.substr(0, 8) == "/uploads")
        return true;
    return false;
}

bool    Client::exists(std::string filePath) {
	if (_path.empty())
		return false;
	std::ifstream fileStream(filePath.c_str());
	bool exists = fileStream.good();
	fileStream.close();
	return exists;
}

void    Client::attemptToRemove(std::string filePath) {
    int removeReturnCode = remove(filePath.c_str());
    if (removeReturnCode == 0)
        _exitState = NO_CONTENT;
    if (removeReturnCode != 0)
        _exitState = INTERNAL_SERVER_ERROR;
}