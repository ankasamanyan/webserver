#include "../includes/sockets.hpp"

// void	recieveRequest(int fd)
// {
// 	string	request;

// 	request.reserve(8000);
// 	recv(fd, (void *)request.c_str(), 8000, 0);
// 	PRINT << SKY "The REQUEST" << RESET_LINE;
// 	PRINT << request << RESET_LINE; 
// }

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

	fd_set	fdSet;
	fd_set	requestSet;
	fd_set	responseSet;
	FD_ZERO(&fdSet);
	int 	highestFd;
	FD_SET(serverSocket, &fdSet);
	highestFd = serverSocket;

	while (true)
	{
		requestSet = responseSet = fdSet;
		int pollReturn = select(highestFd + 1, &requestSet, &responseSet, NULL, 0);

		// PRINT << PURPLE "\t\t......poll returned......" << RESET_LINE;

		for (int fd = 3; fd <= highestFd; fd++)
		{
			if (FD_ISSET(fd, &requestSet))
			{
				if(fd == serverSocket)
				{
					int newFd = accept(serverSocket, (struct sockaddr *)NULL, NULL);
					FD_SET(newFd, &fdSet);
					PRINT << GREEN "\t\t......Someone wants to connect......   ";
					PRINT <<  "FD: "<< newFd << RESET_LINE;
					highestFd = std::max(newFd, highestFd);
					PRINT <<  "newHighest: "<< highestFd << RESET_LINE;
					break ;
				}
				else
				{
					PRINT << YELLOW "\t\t......Client wants to send a REQUEST......   ";
					PRINT <<  "FD: "<< fd << RESET_LINE;
					// recieveRequest(fd);
					char *buffer = (char *)calloc(sizeof(char), 8000);
					string	request;

					recv(fd, buffer, 8000, 0);
					request.append(buffer);
					PRINT << SKY "The REQUEST" << RESET_LINE;
					PRINT << request << RESET_LINE; 
					PRINT << SKY "End of Request" << RESET_LINE;
				}
			}
			else if (FD_ISSET(fd, &responseSet))
			{
				PRINT << PINK "\t\t......Client wants to get a RESPONSE......   ";
				PRINT <<  "FD: "<< fd << RESET_LINE;
			}
			else
			{
				if (fd == serverSocket)
					continue ;
				PRINT << RED "\t\t......SAMSING WEIRD IS HAPPENING......   "; 
				PRINT <<  "FD: "<< fd << RESET_LINE;
				close(fd);
				FD_CLR(fd, &fdSet);
				FD_CLR(fd, &responseSet);
				FD_CLR(fd, &requestSet);
			}
		}
		
	}
	return 0;
}
