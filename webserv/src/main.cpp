#include "../includes/Server_handler.hpp"

int main(int argc, char **argv)
{
    try {
        Configuration *conf = new Configuration(argc, argv);
        PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;
        /* vector parsing structs */
        Server_handler	server(conf->serverRepo);

        while (CPP_IS_PAIN)
        {
            server.serverLoop();
        }
	}
	catch (std::exception& e) {
	    std::cout << e.what() << std::endl;
	}
	return 0;
}
