#include "../includes/Server_handler.hpp"

int main(int argc, char **argv)
{
    Configuration *conf = new Configuration(argc, argv);
    PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;
    /* vector parsing structs */
    Server_handler	server(conf->serverRepo);

    while (CPP_IS_PAIN)
    {
        server.serverLoop();
    }
	return 0;
}
