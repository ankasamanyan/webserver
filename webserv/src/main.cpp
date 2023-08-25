#include "../includes/sockets.hpp"

int main(int argc, char const *argv[])
{
	/* parse config file */
	parsingStruct	innit;
	innit.host = "0.0.0.0";
	innit.port = 2000;
	/* vector parsing structs */
	Server	server(innit);

	
	while (CPP_IS_PAIN)
	{
		// vector<server> serverloop;
		server.serverLoop();
	}
	
	
	return 0;
}
