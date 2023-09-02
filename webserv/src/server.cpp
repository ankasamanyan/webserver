#include "../includes/Server.hpp"


Server::Server(parsingStruct innit)
{
	int option = 1;
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
	_serverAddress.sin_family		= AF_INET;
	_serverAddress.sin_port			= htons(innit.port);
	_serverAddress.sin_addr.s_addr	= inet_addr(innit.host.c_str());

	if (_serverSocket == -1)
		PRINT << RED "Failed Socket init" << RESET_LINE;

	bind(_serverSocket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress));
	listen(_serverSocket, 1024);
	configureSocket(_serverSocket);
    _configuration = innit;
}


Server::~Server(){}

void	Server::configureSocket(int newSocket)
{
	int		flags;

	flags = fcntl(newSocket, F_GETFL, 0);
	if (flags == -1)
		Utils::printMsg("Error configure socket", PINK);
	if (fcntl(newSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		Utils::printMsg("Failed to set socket to non-blocking in fcntl().",PURPLE);
}

void	Server::disconnectClient(fdIter iter)
{
	close(iter->fd);
    _clients.erase(iter->fd);
    _fdVector.erase(iter);
	PRINT << RED "Disconnecting client fd: " << iter->fd << RESET_LINE;
}

int		Server::getSocket()
{
	return(_serverSocket);
}

int		Server::highestFd(std::set<int> activeClients)
{
	return(*(activeClients.rbegin()));
}

void	Server::serverLoop()
{
	pollfd	listenPollFd;
	pollfd	temp;
	fdIter	iter;

	listenPollFd.fd = _serverSocket;
	listenPollFd.events = POLLIN;
	_fdVector.push_back(listenPollFd);

	int pollReturn = poll(_fdVector.data(), _fdVector.size() , -1);
	PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

	for (iter = _fdVector.begin(); iter != _fdVector.end(); iter++)
	{
		if (iter->revents & POLLIN)
		{
			if (iter->fd == _serverSocket)
			{
                acceptClient(iter);
				break;
			}
			/* wants to send a REQUEST */
			PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
			if (receiveRequest(iter) == DISCONNECTED)
				break;
			if (true /* request has ended */)
			iter->events = POLLOUT | POLLHUP;
		}
		else if (iter->revents & POLLOUT)
		{
			string hello("HTTP/1.1 200 OK\r\n\r\nETA CIESCHAZ RABOTAET NORMALNA!");
			send(iter->fd, hello.c_str(), hello.length(), 0);
			PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
			if (true/* if response ended */)
			{
				disconnectClient(iter);
				break;
			}
		}
		else if ( iter->revents & POLLHUP)
		{
			disconnectClient(iter);
			PRINT << ON_PURPLE "\t\t......Client disconnected......   " << RESET_LINE;
			break;
		} 
		else
		{
			if (iter->fd == _serverSocket)
				continue;
			PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
		}
	}
}

void Server::acceptClient(fdIter iter) {
    PRINT <<  "FD: "<< iter->fd << RESET_LINE;
    int clientFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
    configureSocket(clientFd);
    _clients.insert(std::make_pair(clientFd, Client()));

    pollfd	pollFdForThisClient;
    pollFdForThisClient.fd = clientFd;
    pollFdForThisClient.events = POLLIN | POLLHUP;
    _fdVector.push_back(pollFdForThisClient);
    PRINT << GREEN "\t\t......Someone wants to connect......   ";
}

Server::requestState	Server::receiveRequest(fdIter iter) {
    char currentChunk[CHUNK_SIZE];

    memset(currentChunk, 0, CHUNK_SIZE);
    ssize_t numberOfBytesReceived = recv(iter->fd, currentChunk, CHUNK_SIZE, 0);
    if (numberOfBytesReceived == 0) {
        disconnectClient(iter);
        return DISCONNECTED;
    }
    if (numberOfBytesReceived < 0) {
        Utils::printMsg("Error receiving a message from a socket", PURPLE);
    }
    PRINT << SKY "The REQUEST" << RESET_LINE;
    PRINT << currentChunk << RESET_LINE;
    _clients.at(iter->fd).request.append(currentChunk, numberOfBytesReceived);
    if (numberOfBytesReceived < CHUNK_SIZE) {
        if (isRequestValid(iter) == false) {
          return INVALID;
        }
        //ifCGIhandleCGI
        //prepareResponse
        iter->events = POLLOUT;
        return VALID;
    }
    return PARTLY_READ;
}

bool    Server::isRequestValid(fdIter iter) {
    if (isRequestEmpty(iter) == true)
        return false;
    parseRequestLine(iter);
    parseHeaders(iter);
    parseBody(iter);
    return areAllPartsOfRequestValid(iter);
}

bool    Server::isRequestEmpty(fdIter iter) {
    return _clients.at(iter->fd).request.empty();
}

void    Server::parseRequestLine(fdIter iter) {
    Client currentClient = _clients.at(iter->fd);
    std::string	requestLine = currentClient.request.substr(0, currentClient.request.find("\r\n"));
    std::vector<string> splitRequestLine = Utils::split(requestLine, ' ');
    currentClient.method = splitRequestLine[0];
    currentClient.path = splitRequestLine[1];
    currentClient.HTTPVersion = splitRequestLine[2];
}

void    Server::parseHeaders(fdIter iter) {
    Client currentClient = _clients.at(iter->fd);
    int startHeadersIndex = currentClient.request.find('\n') + 1;
    int headersSize = currentClient.request.find("\r\n\r\n") - startHeadersIndex;
    std::string headers = currentClient.request.substr(startHeadersIndex, headersSize);
    std::vector<string> splitHeaders = Utils::split(headers, '\n');
    for (int i = 0; i < splitHeaders.size(); i++) {
        std::string trimmedHeader = Utils::trimRight(splitHeaders[i], "\r\n");
        std::vector<string> headerFieldValue = Utils::split(trimmedHeader, ':');
        headerFieldValue[1] = Utils::trimLeft(headerFieldValue[1], " ");
        currentClient.headers.insert(std::make_pair(headerFieldValue[0], headerFieldValue[1]));
    }
}

void    Server::parseBody(fdIter iter) {
    Client currentClient = _clients.at(iter->fd);
    if (currentClient.method != "POST")
        return ;
    size_t startBodyIndex = currentClient.request.find("\r\n\r\n") + 4;
    size_t bodySize;
    if (currentClient.headers.find("Content-Length") != currentClient.headers.end()) {
        bodySize = atoi(currentClient.headers.at("Content-Length").c_str());
    }
    else {
        bodySize = string::npos;
    }
    currentClient.body = currentClient.request.substr(startBodyIndex, bodySize);
}

bool    Server::areAllPartsOfRequestValid(fdIter iter) {
    if (isMethodAllowed(iter) == false)
        return false;
    if (isHTTPVersionValid(iter) == false)
        return false;
    return true;
}

bool    Server::isMethodAllowed(fdIter iter) {
    Client currentClient = _clients.at(iter->fd);
    if (currentClient.method == "GET" && _configuration.methodGet)
        return true;
    else if (currentClient.method == "DELETE" && _configuration.methodDelete)
        return true;
    else if (currentClient.method == "POST" && _configuration.methodPost)
        return true;
    return false;
}

bool    Server::isHTTPVersionValid(fdIter iter) {
    Client currentClient = _clients.at(iter->fd);
    if (currentClient.HTTPVersion == "HTTP/1.1")
        return true;
    return false;
}
