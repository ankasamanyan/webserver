#include "../includes/sockets.hpp"


Server::Server(parsingStruct innit)
{
	struct sockaddr_in	_serverAddress;
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	int option = 1;

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

	_serverAddress.sin_family		= AF_INET;
	_serverAddress.sin_port			= htons(innit.port);
	_serverAddress.sin_addr.s_addr	= inet_addr(innit.host.c_str());

	if (_serverSocket == -1)
		PRINT << RED "Failed Socket init" << RESET_LINE;

	bind(_serverSocket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress));
	listen(_serverSocket, 1024);
	FD_ZERO(&_fdSet);
	FD_SET(_serverSocket, &_fdSet);
	_activeClients.insert(_serverSocket);
}

Server::~Server(){}

void	Server::disconnectClient(int fd)
{
	close(fd);
	FD_CLR(fd, &_fdSet);
	_activeClients.erase(fd);
	PRINT << RED "Disconnecting client fd: " << fd << RESET_LINE;
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
	fd_set 	requestSet;
	fd_set	responseSet;
	FD_ZERO(&requestSet);
	FD_ZERO(&responseSet);
	// PRINT << ORANGE "_activeClients.rbegin():  " << highestFd(_activeClients) << RESET_LINE;
	requestSet = responseSet = _fdSet;
	int pollReturn = select(highestFd(_activeClients) + 1, &requestSet, &responseSet, NULL, 0);

	PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

	for (int fd = 3; fd <= highestFd(_activeClients); fd++)
	{
		if (FD_ISSET(fd, &requestSet))
		{
			if(fd == _serverSocket)
			{
				int newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
				_activeClients.insert(newFd);
				FD_SET(newFd, &_fdSet);
				PRINT << GREEN "\t\t......Someone connected......   ";
				PRINT <<  "FD: "<< newFd << RESET_LINE;
				break ;
			}
			else
			{
				PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
				PRINT <<  "FD: "<< fd << RESET_LINE;
				recieveRequest(fd);
			}
		}
		else if (FD_ISSET(fd, &responseSet))
		{
			string hello("HTTP/1.1 200 OK\r\n\r\nETA RABOTAET RANDOMNDA!");
			send(fd, hello.c_str(), hello.length(), 0);
			PRINT << PINK "\t\t......Client wants to get a RESPONSE......";
			PRINT <<  "FD: "<< fd << RESET_LINE;
			disconnectClient(fd);
		}
		else
		{
			if (fd == _serverSocket)
				continue ;
			PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
			PRINT <<  "FD: "<< fd << RESET_LINE;
			disconnectClient(fd);
		}
	}
}

int	Server::recieveRequest(int fd)
{
	string	request;
	char *buffer = (char *)calloc(sizeof(char), 8000);
	int	recAmount;

	recAmount = recv(fd, buffer, 8000, 0);
	if (recAmount == 0)
	{
		disconnectClient(fd);
		return -1;
	}
	request.append(buffer);
	PRINT << SKY "The REQUEST" << RESET_LINE;
	PRINT << request << RESET_LINE; 
	return 0;
}