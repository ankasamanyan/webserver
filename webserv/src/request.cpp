#include "../includes/Server_handler.hpp"

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
    setDefaultFile();
    updateDirectoryIfUploading();
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
    PRINT << SKY "PATH" << _path << "DIRECTORY" << _directory << "FILE" << _file << RESET_LINE;
}

std::string Client::getDirectory() {
    std::string pathWithRoot = getConfig().root + _path;
    if (isDirectory(pathWithRoot) && _path[_path.size() - 1] != '/')
        _path.append("/");
    return _path.substr(0, _path.substr(1).find_first_of("/") + 2);
}

std::string Client::getFile() {
    std::string file;
    if (_path.find_last_of("/") != string::npos && _path != "/")
        file = _path.substr(_path.substr(1).find_first_of(_directory) + _directory.size());
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
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

	if (it != getConfig().locations.end() && _method == "POST" && !it->second.uploadsDir.empty())
		_directory = it->second.uploadsDir;
}

void    Client::setDefaultFile() {
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

    if (it != getConfig().locations.end() && !it->second.defaultFile.empty())
        _defaultFile = getConfig().root + it->second.defaultFile;
    else
        _defaultFile = "/html/errorHtml/404.html";
}

void    Client::defineRequestTarget() {
    _directoryListingCase = false;
    _requestTarget = getConfig().root + _directory.substr(1) + _file;
    redirectIfNeeded();
    if (isDirectory(_requestTarget) && _method == "GET" && _requestTarget != getConfig().root)
        assignContent();
}

void    Client::redirectIfNeeded() {
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

    if (it != getConfig().locations.end() && !it->second.redirection.empty()) {
        _requestTarget = getConfig().root + it->second.redirection.substr(1) + _file;
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
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

    if (it != getConfig().locations.end() && it->second.dirListing == false)
        _requestTarget = _defaultFile;
    else
        _directoryListingCase = true;
}

void    Client::assignCGIFlag() {
    int CGIDirectoryLength = getConfig().CGIDir.length();
    std::string partOfPath = _path.substr(0, CGIDirectoryLength);
    if (getConfig().CGIDir == partOfPath)
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

bool Client::isPathAllowed() {
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

    if (it == getConfig().locations.end()) {
        assignErrorForInvalidPath();
        return false;
    }
    return true;
}

void    Client::assignErrorForInvalidPath() {
    std::string fileToCheck = "." + getConfig().root + _directory.substr(1);
    if (exists(fileToCheck))
        _exitState = FORBIDDEN;
    else
        _exitState = ERROR_404;
}

bool    Client::isMethodAllowed() {
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);
	_location = it->second;

    if ((_method == "GET" && _location.methodGet) ||
        (_method == "DELETE" && _location.methodDelete) ||
        (_method == "POST" && _location.methodPost))
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
    int allowedSize = atoi(getConfig().maxBody.c_str());

    if (_headers.find("Content-Length") != _headers.end()) {
        int contentSize = atoi(_headers.at("Content-Length").c_str());
        if (contentSize <= allowedSize && (size_t) contentSize < _body.max_size())
            return true;
    }
    else {
        int actualBodySize = _body.length();
        if (actualBodySize <= allowedSize && (size_t) actualBodySize < _body.max_size())
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
        std::string pathToPost = "." + _requestTarget;
        std::ofstream outputFileStream;
        outputFileStream.open((pathToPost).c_str(), std::ios::binary | std::ios::app);
        outputFileStream.write(_body.c_str(), _body.length());
        outputFileStream.close();
    }
}

void    Client::handleDelete() {
    std::string fileToDelete = "." + _requestTarget;

    if (isAllowedToDelete() == false)
        _exitState = FORBIDDEN;
    else if (exists(fileToDelete) == false)
        _exitState = ERROR_404;
    else
        attemptToRemove(fileToDelete);
}

bool    Client::isAllowedToDelete() {
    return !(isDirectory(_requestTarget) || isInsideUploads() == false);
}

bool    Client::isInsideUploads() {
    if (_requestTarget.find("/uploads") != std::string::npos)
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