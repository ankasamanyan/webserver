#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
	Configuration *conf = new Configuration(argc, argv);

	PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;
	// parsingStruct	innit;
	// innit.host = "0.0.0.0";
	// innit.port = 2000;
    // innit.methodDelete = true;
    // innit.methodPost = true;
    // innit.methodGet = true;
    // innit.maxBody = "8000";
    // innit.root = "/html";
    // innit.dirListing = true;
    // innit.CGIDir = "";
	/* vector parsing structs */
	Server	server(conf->config);
	while (CPP_IS_PAIN)
	{
		server.serverLoop();
	}
	return 0;
}
