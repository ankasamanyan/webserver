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
    PRINT << currentChunk << RESET_LINE;
  	_request.append(currentChunk, numberOfBytesReceived);
    if (numberOfBytesReceived < CHUNK_SIZE) 
	{

        if (isRequestValid() == false) {
			_clientState = INVALID_;
			return ;
        }
        //ifCGIhandleCGI
        //prepareResponse
		
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
    defineRequestTarget();
    return areAllPartsOfRequestValid();
}

bool    Client::isRequestEmpty() {
    _exitState = BAD_REQUEST;
    return _request.empty();
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

void    Client::defineRequestTarget() {
    _requestTarget = _configuration.root + _path;
    if (isRequestTargetDirectory())
            assignContent();
}

bool    Client::isRequestTargetDirectory() {
    struct stat buffer;
    std::string filePath = "." + _requestTarget;
    if (stat(filePath.c_str(), &buffer) == 0)
        return S_ISDIR(buffer.st_mode);
    return false;
}

void    Client::assignContent() {
    _requestTarget = "/html/errorHtml/404.html";
}

bool    Client::areAllPartsOfRequestValid() {
    if (isMethodAllowed() == false)
        return false;
    if (isHTTPVersionValid() == false)
        return false;
    if (isContentOfAllowedSize() == false)
        return false;
    return true;
}

bool    Client::isMethodAllowed() {
    if (_method == "GET" && _configuration.methodGet)
        return true;
    else if (_method == "DELETE" && _configuration.methodDelete)
        return true;
    else if (_method == "POST" && _configuration.methodPost)
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
    if (_headers.find("Content-Length") != _headers.end()) {
        int contentSize = atoi(_headers.at("Content-Length").c_str());
        int actualBodySize = _body.length();
        int allowedSize = atoi(_configuration.maxBody.c_str());
        if (contentSize <= allowedSize && actualBodySize <= allowedSize)
            return true;
        _exitState = CONTENT_TOO_LARGE;
        return false;
    }
    return true;
}

clientState         Client::getState()
{
	return (_clientState);
}
