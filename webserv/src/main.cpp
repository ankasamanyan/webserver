#include "../includes/sockets.hpp"

int main(int argc, char const *argv[])
{
	parsingStruct	innit;
	innit.host = "0.0.0.0";
	innit.port = 2000;
	/* vector parsing structs */
	Server	server(innit);
	while (CPP_IS_PAIN)
	{
		server.serverLoop();
	}
	return 0;
}
