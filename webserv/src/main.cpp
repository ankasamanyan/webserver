#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

int main(int argc, char const *argv[])
{
	(void)argv;
	(void)argc;
	PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;
	parsingStruct	innit;
	innit.host = "0.0.0.0";
	innit.port = 2000;
    innit.methodDelete = true;
    innit.methodPost = true;
    innit.methodGet = true;
    innit.maxBody = "8000";
	/* vector parsing structs */
	Server	server(innit);
	while (CPP_IS_PAIN)
	{
		server.serverLoop();
	}
	return 0;
}
