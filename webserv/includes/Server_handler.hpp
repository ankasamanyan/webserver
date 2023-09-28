#ifndef SERVER_HPP
	#define SERVER_HPP

#include <poll.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "Utils.hpp"
#include <sys/select.h>
#include <vector>
#include <fcntl.h>
#include <set>
#include "Client.hpp"
#include "Configuration.hpp"
#include <map>
#include <sstream>
#include <cstdlib>
#include <errno.h>

#define CHUNK_SIZE 	8000
#define HTTP_V 		"HTTP/1.1"
#define EXIT_STATUS	200

using std::string;
class Client;

class Server_handler
{
	friend class Client;

	private:
		struct sockaddr_in										_serverAddress;
		size_t													_serverAmount;
		std::vector<pollfd>										_fdVector;
		std::map<int, Client>   								_clients;
		std::map<int, std::map<std::string, configuration> >	_serverConfigMap;


		/* defines */
		enum requestState
		{
			VALID,
            PARTLY_READ,
            INVALID,
			DISCONNECTED,
			SHOULD_DISCONNECT,
			REQUEST_ENDED,
		};
		typedef std::vector<pollfd>::iterator fdIter;
		/* functions */
        void                acceptClient(fdIter iter);
		requestState		receiveRequest(fdIter iter);
		int					highestFd(std::set<int> activeClients);
		void				disconnectClient(fdIter iter);
		void				pushConfigsIntoMap(configuration, std::map<std::string, int> &);
		int					createNewListener(configuration config);


    public:
		Server_handler(std::deque<configuration> innit);
		~Server_handler();
		/* functions */
		int		getSocket();
		void	serverLoop();
		void	configureSocket(int newSocket);
};




#endif
