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
	_fdVector.erase(iter);
	PRINT << RED "Disconnecting client fd: " << iter->fd << RESET_LINE;
}

Server::requestState	Server::recieveRequest(fdIter iter)
{
	string	request;
	char *buffer = (char *)calloc(sizeof(char), 8000);
	int	recAmount;

	recAmount = recv(iter->fd, buffer, 8000, 0);
	if (recAmount == 0)
	{
		disconnectClient(iter);
		return DISCONNECTED;
	}
	request.append(buffer);
	PRINT << SKY "The REQUEST" << RESET_LINE;
	PRINT << request << RESET_LINE; 
	iter->events = POLLOUT;
	return VALID;
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
				int newFd;
				PRINT <<  "FD: "<< iter->fd << RESET_LINE;
				newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
				configureSocket(newFd);
				temp.fd = newFd;
				temp.events = POLLIN | POLLHUP;
				_fdVector.push_back(temp);
				PRINT << GREEN "\t\t......Someone wants to connect......   ";
				break;
			}
			/* wants to send a REQUEST */
			PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
			if (recieveRequest(iter) == DISCONNECTED)
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
