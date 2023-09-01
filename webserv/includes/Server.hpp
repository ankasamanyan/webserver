#ifndef SERVER_HPP
	#define SERVER_HPP

#include <poll.h>
#include <string>
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

using std::string;

struct parsingStruct{
	string	host;
	int		port;
	// string	rootDir;
};


class Server
{
	private:
		struct sockaddr_in		_serverAddress;
		int						_serverSocket;
		std::vector<pollfd>		_fdVector;
		std::map<int, Client>   _clients;

		/* defines */
		enum requestState
		{
			VALID,
			DISCONNECTED
		};
		typedef std::vector<pollfd>::iterator fdIter;
		/* functions */
        void                acceptClient(fdIter iter);
		void				disconnectClient(fdIter iter);
		requestState		receiveRequest(fdIter iter);
		int					highestFd(std::set<int> activeClients);


	public:
		/* Server(parsingStruct init) */
		Server(parsingStruct innit);
		~Server();
	/* functions */
		int		getSocket();
		void	serverLoop();
		void	configureSocket(int newSocket);


};




#endif
