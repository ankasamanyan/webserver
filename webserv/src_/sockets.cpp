#include "sockets.hpp"

WBSRV::simpleSocket::simpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
	/* Define address structure */
	_address.sin_family = domain;
	_address.sin_port = htons(port); 
	_address.sin_addr.s_addr = htonl(interface);
	/* Establish connection */
	_socket = socket(domain, service, protocol);
	/* bind */
	_connection = connectToNetwork(_socket, _address);
	testConnection(_connection);
}

void	WBSRV::simpleSocket::testConnection(int itemToTest)
{
	if (itemToTest < 0)
	{
		perror(RED "... Connection failed ...");
		exit(EXIT_FAILURE); 
	}	
}

struct sockaddr_in	WBSRV::simpleSocket::getAddress()
{
	return (_address);
}

int	WBSRV::simpleSocket::getSocket()
{
	return (_socket);
}

int	WBSRV::simpleSocket::getConnection()
{
	return (_connection);
}
