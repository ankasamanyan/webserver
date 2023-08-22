#ifndef SOCKETS
	#define SOCKETS

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

using std::string;
//  struct pollfd {
//                int   fd;         /* file descriptor */
//                short events;     /* requested events */
//                short revents;    /* returned events */
//            };
namespace WBSRV
{
	class simpleSocket
	{
		private:
			int						_socket;
			int						_connection;

		public:
			simpleSocket(/* args */);
			~simpleSocket();
			simpleSocket(int domain, int service, int protocol, int port, u_long interface);
			/*  */
			virtual int 		connectToNetwork(int sock, struct sockaddr_in address) = 0;
			void 				testConnection(int);
			/* Getters */
			struct sockaddr_in	getAddress();
			int					getSocket();
			int					getConnection();
	};
}
#endif

// 127.0.0.1
// 
// 
// 