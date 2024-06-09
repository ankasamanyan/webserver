#include "../includes/Server_handler.hpp"

int main(int argc, char **argv)
{
    Configuration *conf = new Configuration(argc, argv);

    PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;

    if (!conf->config.port.empty()) {
        PRINT << RESET_LINE << SKY << "\t\t... PORT: " << "127.0.0.1:"<< conf->config.port << "  ..." << RESET_LINE;
    }

    Server_handler	server(conf->serverRepo);
    while (CPP_IS_PAIN)
	{
        server.serverLoop();
	}
	delete conf;
	return 0;
}
