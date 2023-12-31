#include "../includes/Server_handler.hpp"


Server_handler::Server_handler(std::deque<configuration> innit)
{
	std::map<std::string, int>	tempMap;
	_serverAmount = 0;

	while (!innit.empty())
	{
		pushConfigsIntoMap(innit.front(), tempMap);
		innit.pop_front();
	}
}

void	Server_handler::pushConfigsIntoMap(configuration	config, std::map<std::string, int> &mapForCheck)
{
	int tempSocket;

	if (mapForCheck.find(config.host + config.port) != mapForCheck.end())
	{
		tempSocket = mapForCheck.at(config.host + config.port);
		std::map<std::string, configuration> &anotherConfigForThisSocket = _serverConfigMap.at(tempSocket);
		anotherConfigForThisSocket.insert(std::make_pair(config.serverName, config));
	}
	else 
	{
		tempSocket = createNewListener(config);

		std::map<std::string, configuration>	tempMap;
		tempMap.insert(std::make_pair("default", config));
		tempMap.insert(std::make_pair(config.serverName, config));
		_serverConfigMap.insert(std::make_pair(tempSocket, tempMap));

		mapForCheck.insert(std::make_pair(config.host + config.port, tempSocket));
	}
}

int		Server_handler::createNewListener(configuration config)
{
	int		option = 1;
	int		newListenerSocket;
	pollfd	listenPollFd;
	newListenerSocket = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(newListenerSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	_serverAddress.sin_family		= AF_INET;
	_serverAddress.sin_port			= htons(atoi((config.port).c_str()));
	_serverAddress.sin_addr.s_addr	= inet_addr(config.host.c_str());

	if (newListenerSocket == -1)
	{
		PRINT << RED "Failed Socket init" << RESET_LINE;
		exit(1);
	}
	if (bind(newListenerSocket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		PRINT << RED "Failed Socket bind" << RESET_LINE;
		exit(1);
	}
	if (listen(newListenerSocket, 1024) == -1)
	{
		PRINT << RED "Failed Socket listen" << RESET_LINE;
		exit(1);
	}

	configureSocket(newListenerSocket);
	listenPollFd.fd = newListenerSocket;
	listenPollFd.events = POLLIN;
	_fdVector.push_back(listenPollFd);
	_serverAmount++;

	return (newListenerSocket);
}

Server_handler::~Server_handler(){}

void	Server_handler::configureSocket(int newSocket)
{
	int		flags;

	flags = fcntl(newSocket, F_GETFL, 0);
	if (flags == -1)
		Utils::printMsg("Error configure socket", PINK);
	if (fcntl(newSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		Utils::printMsg("Failed to set socket to non-blocking in fcntl().",PURPLE);
}

void	Server_handler::disconnectClient(fdIter iter)
{
	close(iter->fd);
    _clients.erase(iter->fd);
    _fdVector.erase(iter);
}

int		Server_handler::highestFd(std::set<int> activeClients)
{
	return(*(activeClients.rbegin()));
}

void	Server_handler::serverLoop()
{
	fdIter	iter = _fdVector.begin();
	bool	newClientAdded = false;

	poll(_fdVector.data(), _fdVector.size() , -1);
	for (size_t i = 0; i < _serverAmount; i++, iter++)
	{
		if(iter->revents & POLLIN)
		{
            acceptClient(iter);
			newClientAdded = true;
			break;
		}
	}
	if (newClientAdded)
		return;
	for (iter = _fdVector.begin() + _serverAmount; iter != _fdVector.end(); iter++)
	{
	    Client	&currClient =_clients.at(iter->fd);
		if (iter->revents & POLLIN)
		{
			currClient.receiveRequest();
			if(currClient.getState() == DONE_) {
       			iter->events = POLLOUT| POLLHUP;
       	    }
			if ( currClient.getState() == SHOULD_DISCONNECT_)
			{
				disconnectClient(iter);
				break;
			}
		}
		else if (iter->revents & POLLOUT)
		{
			if (currClient._CGICase == true)
				currClient.CgiParentHandler();
			else
				currClient.sendResponse();
			if (currClient._responseState == FULLY_SENT ||  currClient.getState() == SHOULD_DISCONNECT_)
				disconnectClient(iter);
			break;
		}
		else if ( iter->revents & POLLHUP)
		{
			disconnectClient(iter);
			break;
		}
		else
		{

		}
	}
}

void Server_handler::acceptClient(fdIter iter) {
	Client newClient(iter->fd, _serverConfigMap.at(iter->fd));
    _clients.insert(std::make_pair(newClient._clientFd, newClient));

    pollfd	pollFdForThisClient;
    pollFdForThisClient.fd = newClient._clientFd;
    pollFdForThisClient.events = POLLIN | POLLHUP;
    _fdVector.push_back(pollFdForThisClient);
}
