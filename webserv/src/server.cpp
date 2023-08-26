#include "../includes/sockets.hpp"


Server::Server(parsingStruct innit)
{
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

}

Server::~Server(){}

void	Server::disconnectClient(int fd)
{
	// close(fd);
	// FD_CLR(fd, &_fdSet);
	// _activeClients.erase(fd);
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

	int 	fdAmount = 2;
	pollfd	*fds;
	pollfd	listenPollFd;
	pollfd	*fdStructs;

	listenPollFd.fd = _serverSocket;
	listenPollFd.events = POLL_IN;
	fds[0] = listenPollFd;

	int pollReturn = poll(_fdVector.data(), fdAmount ,-1);
	// PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

	for (int i = 0; i < fdAmount; i++)
	{
		if (fds[i].revents & POLL_IN)
		{
			if (fds[i].fd == _serverSocket)
			{
				int newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
				fds[1].fd = newFd;
				fds[1].events = POLL_IN;
				PRINT << GREEN "\t\t......Someone wants to connect......   ";
				PRINT <<  "FD: "<< fds[1].fd << RESET_LINE;
				fdAmount++;
				break;
			}
			// else
			// {
				/* wants to send a REQUEST */
				PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
				PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
				recieveRequest(fds[i].fd);
			// }
		}
		else if (fds[i].revents & POLL_OUT)
		{
			string hello("HTTP/1.1 200 OK\r\n\r\nETA RABOTAET RANDOMNDA!");
			send(fds[i].fd, hello.c_str(), hello.length(), 0);
			PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
			PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
		}
		else if (fds[i].revents & POLL_HUP)
		{
			close(fds[i].fd);
			// fdAmount--;
			PRINT << ON_PURPLE "\t\t......Client disconnected......   " << RESET_LINE;
		} 
		else
		{
			if (fds[i].fd == _serverSocket)
				continue;
			// PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
			// PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
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