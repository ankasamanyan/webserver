#include "../includes/Server_handler.hpp"

void	Client::receiveRequest() 
{
    char currentChunk[CHUNK_SIZE];

    memset(currentChunk, 0, CHUNK_SIZE);
    ssize_t numberOfBytesReceived = recv(_clientFd, currentChunk, CHUNK_SIZE, 0);
	if (numberOfBytesReceived == 0) {
		_clientState = SHOULD_DISCONNECT_;
		return ; 
	}
    if (numberOfBytesReceived < 0) {
        Utils::printMsg("Error receiving a message from a socket", PURPLE);
    }
    PRINT << SKY "The REQUEST" << RESET_LINE;
	// PRINT << currentChunk << RESET_LINE;
  	_request.append(currentChunk, numberOfBytesReceived);
    if (_method.empty())
        processRequest();
    prepareResponse();
    if (_CGICase == true && _clientState == DONE_) {
        handleCGI();
    }
}

void    Client::processRequest() {
    parseRequestIfNotEmpty();
    assignPossibleErrorCodes();
    continueToProcessIfStillValid();
}

void    Client::parseRequestIfNotEmpty() {
    if (_request.empty()) {
        _exitState = BAD_REQUEST;
        return ;
    }
    parseRequestLine();
    parseHeaders();
}

void    Client::parseRequestLine() {
    extractRequestLine();
    splitRequestLine();
    definePartsOfURL();
}

void    Client::extractRequestLine() {
    _requestLine = _request.substr(0, _request.find("\r\n"));
    _request.erase(0, _request.find("\r\n") + 2);
}

void    Client::splitRequestLine() {
    std::vector<string> splitRequestLine = Utils::split(_requestLine, ' ');
    _method = splitRequestLine[0];
    _path = splitRequestLine[1];
    _HTTPVersion = splitRequestLine[2];
}

void    Client::definePartsOfURL() {
    _directory = getDirectory();
    _query = getQuery();
    _file = getFile();
}

std::string Client::getDirectory() {
    std::string pathWithRoot = getConfig().root + _path;
    if (isDirectory(pathWithRoot) && _path[_path.size() - 1] != '/')
        _path.append("/");
    return _path.substr(0, _path.substr(1).find_first_of("/") + 2);
}

std::string Client::getFile() {
    return Utils::trimLeft(_path.substr(_path.substr(1).find_first_of(_directory) + _directory.size()), "/");
}

std::string Client::getQuery() {
	size_t	questionMark = _path.find("?");
	std::string query;

	if (questionMark != std::string::npos) {
		query = _path.substr(questionMark + 1);
		_path = _path.substr(0, questionMark);
	}
	else
	    query = "";
	return query;
}

void    Client::parseHeaders() {
    extractHeaders();
    std::vector<string> splitHeaders = Utils::split(_headersChunk, '\n');
    for (size_t i = 0; i < splitHeaders.size(); i++) {
        std::string trimmedHeader = Utils::trimRight(splitHeaders[i], "\r\n");
        std::vector<string> keyValue = Utils::split(trimmedHeader, ':');
        _headers.insert(std::make_pair(keyValue[0], Utils::trimLeft(keyValue[1], " ")));
    }
    defineServerName();
}

void    Client::extractHeaders() {
    int headersSize = _request.find("\r\n\r\n");
    _headersChunk = _request.substr(0, headersSize);
    _request.erase(0, headersSize + 4);
}

void    Client::defineServerName() {
	if (_headers.find("Server-Name") != _headers.end()) {
	    _requestedServerName = _headers.at("Server-Name").c_str();
	}
}

void    Client::assignPossibleErrorCodes() {
    if (_exitState == EXIT_OK) {
        checkPathIsAllowed();
        checkMethodIsAllowed();
        checkHTTPVersionIsValid();
        checkContentIsOfAllowedSize();
    }
}

void Client::checkPathIsAllowed() {
    std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);

    if (it == getConfig().locations.end()) {
        assignErrorForInvalidPath();
    }
}

void    Client::assignErrorForInvalidPath() {
    std::string fileToCheck = "." + getConfig().root + _directory.substr(1);
    if (exists(fileToCheck))
        _exitState = FORBIDDEN;
    else
        _exitState = ERROR_404;
}

void    Client::checkMethodIsAllowed() {
    if (_exitState == EXIT_OK) {
        std::map<std::string, location>::const_iterator it = getConfig().locations.find(_directory);
        _location = it->second;

        if ((_method == "GET" && _location.methodGet) ||
            (_method == "DELETE" && _location.methodDelete) ||
            (_method == "POST" && _location.methodPost))
            return ;
        _exitState = METHOD_NOT_ALLOWED;
    }
}

void    Client::checkHTTPVersionIsValid() {
    if (_exitState == EXIT_OK) {
        if (_HTTPVersion == "HTTP/1.1")
            return ;
        _exitState = INVALID_HHTPV;
    }
}

void    Client::checkContentIsOfAllowedSize() {
    if (_exitState == EXIT_OK) {
        size_t allowedSize = atoi(getConfig().maxBody.c_str());

        if (_headers.find("Content-Length") != _headers.end()) {
            _contentLength = atoi(_headers.at("Content-Length").c_str());
            if (_contentLength <= allowedSize)
                return ;
            else {
                _exitState = CONTENT_TOO_LARGE;
                return ;
            }
        }
        _contentLength = 0;
    }
}

void    Client::continueToProcessIfStillValid() {
    if (_exitState == EXIT_OK) {
        setDefaultFile();
        updateDirectoryIfUploading();
        defineRequestTarget();
        assignCGIFlag();
    }
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
    else
        _clientState = DONE_;
}

void    Client::handleGet() {
    _request.clear();
    _clientState = DONE_;
}

void    Client::handlePost() {
    checkIsInsideUploads();
    createFileIfAllowed();
    if (_bytesWritten >= _contentLength)
        _clientState = DONE_;
}

void    Client::checkIsInsideUploads() {
    if (_requestTarget.find("/uploads") != std::string::npos)
        return ;
    _exitState = FORBIDDEN;
}

void    Client::createFileIfAllowed() {
    std::ofstream outputFileStream;
    std::string pathToPost = "." + _requestTarget;
    if (_exitState == EXIT_OK) {
        if (_shouldAppend)
            outputFileStream.open((pathToPost).c_str(), std::ios::binary | std::ios::app);
        else {
            outputFileStream.open((pathToPost).c_str(), std::ios::binary | std::ios::trunc);
            _shouldAppend = true;
        }
        outputFileStream.write(_request.c_str(), _request.size());
    }
    _bytesWritten += _request.size();
    _request.clear();
    if (_exitState == EXIT_OK) {
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
    _request.clear();
    _clientState = DONE_;
}

bool    Client::isAllowedToDelete() {
    checkIsInsideUploads();
    return !(isDirectory(_requestTarget) || _exitState == FORBIDDEN);
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
