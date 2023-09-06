#include "../includes/Server.hpp"


Server::Server(parsingStruct innit)
{
	int option = 1;
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	_serverAddress.sin_family		= AF_INET;
	_serverAddress.sin_port			= htons(innit.port);
	_serverAddress.sin_addr.s_addr	= inet_addr(innit.host.c_str());

	if (_serverSocket == -1)
	{
		PRINT << RED "Failed Socket init" << RESET_LINE;
		exit(1);
	}
	if (bind(_serverSocket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		PRINT << RED "Failed Socket bind" << RESET_LINE;
		exit(1);
	}
	if (listen(_serverSocket, 1024) == -1)
	{
		PRINT << RED "Failed Socket listen" << RESET_LINE;
		exit(1);
	}
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
	fdIter	iter;

	listenPollFd.fd = _serverSocket;
	listenPollFd.events = POLLIN;
	_fdVector.push_back(listenPollFd);

	poll(_fdVector.data(), _fdVector.size() , -1);
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
			Client	&currClient =_clients.at(iter->fd);
			currClient.receiveRequest();
			if(currClient.getState() == VALID_)
       			 iter->events = POLLOUT;
			if ( currClient.getState() == SHOULD_DISCONNECT_)
			{
				disconnectClient(iter);
				break;
			}
			/* wants to send a REQUEST */
			PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
			if (REQUEST_ENDED/* add the condition */)
				iter->events = POLLOUT | POLLHUP;
		}
		else if (iter->revents & POLLOUT)
		{
			PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
			PRINT <<  "FD: "<< iter->fd << RESET_LINE;
			Client	&currClient =_clients.at(iter->fd);
			if (true/* if response ended */)
			{
				currClient.sendResponse();
				if (currClient._responseState == FULLY_SENT)
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
	Client newClient(iter->fd, _configuration);
    _clients.insert(std::make_pair(newClient._clientFd, newClient));

    pollfd	pollFdForThisClient;
    pollFdForThisClient.fd = newClient._clientFd;
    pollFdForThisClient.events = POLLIN | POLLHUP;
    _fdVector.push_back(pollFdForThisClient);
    PRINT << GREEN "\t\t......Someone wants to connect......   ";
}

