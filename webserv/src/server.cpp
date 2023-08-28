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

void	Server::disconnectClient(fdIter iter)
{
	// FD_CLR(fd, &_fdSet);
	// _activeClients.erase(fd);
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

	// pollfd	*fds;
	pollfd	listenPollFd;
	// pollfd	temp;
	fdIter	iter;
	// pollfd	*fdStructs;


	listenPollFd.fd = _serverSocket;
	listenPollFd.events = POLL_IN;
	_fdVector.push_back(listenPollFd);

	int 	fdAmount = _fdVector.size();
	int pollReturn = poll(_fdVector.data(), fdAmount ,-1);
	PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

	for (iter = _fdVector.begin(); iter != _fdVector.end(); iter++)
	{
		if ((*iter).revents & POLL_IN)
		{
			if ((*iter).fd == _serverSocket)
			{
				PRINT <<  "FD: "<< (*iter).fd << RESET_LINE;
				int newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
				pollfd *temp = new pollfd;
				temp->fd = newFd;
				temp->events = POLL_IN;
				_fdVector.push_back(*temp);
				PRINT << GREEN "\t\t......Someone wants to connect......   ";
				fdAmount++;
				break;
			}
			// else
			// {
				/* wants to send a REQUEST */
				PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
				PRINT <<  "FD: "<< iter->fd << RESET_LINE;
				if (recieveRequest(iter) == DISCONNECTED)
					break;
				
			// }
		}
		else if ((*iter).revents & POLL_OUT)
		{
			string hello("HTTP/1.1 200 OK\r\n\r\nETA RABOTAET RANDOMNDA!");
			send((*iter).fd, hello.c_str(), hello.length(), 0);
			PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
			PRINT <<  "FD: "<< (*iter).fd << RESET_LINE;
		}
		else if ( (*iter).revents & POLL_HUP)
		{
			close((*iter).fd);
			// fdAmount--;
			PRINT << ON_PURPLE "\t\t......Client disconnected......   " << RESET_LINE;
		} 
		else
		{
			if ((*iter).fd == _serverSocket)
				continue;
			PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
			PRINT <<  "FD: "<< (*iter).fd << RESET_LINE;
		}
	}
}
