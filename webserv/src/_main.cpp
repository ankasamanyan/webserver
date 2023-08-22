#include "../includes/sockets.hpp"

int main(int argc, char const *argv[])
{
	int	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	int option = 1;

	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

	struct sockaddr_in	_serverAddress;

	_serverAddress.sin_family		= AF_INET;
	_serverAddress.sin_port			= htons(2000);
	_serverAddress.sin_addr.s_addr	= inet_addr("127.0.0.1");

	if (serverSocket == -1)
		PRINT << RED "Failed Socket init" << RESET_LINE;

	bind(serverSocket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress));
	listen(serverSocket, 1024);


	int 	fdAmount = 1;
	pollfd	fds[2];
	pollfd	listenPollFd;
	pollfd	*fdStructs;

	listenPollFd.fd = serverSocket;
	listenPollFd.events = POLL_IN;
	fds[0] = listenPollFd;

	while (true)
	{
		// PRINT << SKY "\t\t......I got here......" << RESET_LINE;
		int pollReturn = poll(fds, fdAmount ,-1);
		PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

		for (int i = 0; i < fdAmount; i++)
		{
			if (fds[i].revents & POLL_IN)
			{
				if (fds[i].fd == serverSocket)
				{
					int newFd = accept(serverSocket, (struct sockaddr *)NULL, NULL);
					fds[1].fd = newFd;
					fds[1].events = POLL_IN;
					PRINT << GREEN "\t\t......Someone wants to connect......   ";
					PRINT <<  "FD: "<< fds[1].fd << RESET_LINE;
					fdAmount++;
					break;
				}
				/* wants to send a REQUEST */
				PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
				PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
				
			}
			else if (fds[i].revents & POLL_OUT)
			{
				PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
				PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
			}
			else
			{
				if (fds[i].fd == serverSocket)
					continue;
				PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
				PRINT <<  "FD: "<< fds[i].fd << RESET_LINE;
			}
		}
		
	}
	return 0;
}
