#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
	Configuration *conf = new Configuration(argc, argv);

	PRINT << RESET_LINE << PURPLE "\t\t... STARTING THE SERVER ..." << RESET_LINE;

	conf->config.host = "0.0.0.0";
	conf->config.port = "2000";
    conf->config.locations["/"].methodDelete = true;
    conf->config.locations["/"].methodGet = true;
    conf->config.locations["/"].methodPost = true;
    conf->config.maxBody = "80000";
    conf->config.root = "/html/";
    conf->config.locations["/uploads/"].methodPost = true;
    conf->config.locations["/uploads/"].methodGet = true;
    conf->config.locations["/uploads/"].methodDelete = true;
    conf->config.locations["/uploads/"].dirListing = true;

    conf->config.locations["/errorHtml/"].methodPost = false;
    conf->config.locations["/errorHtml/"].methodGet = true;
    conf->config.locations["/errorHtml/"].methodDelete = false;
    conf->config.locations["/errorHtml/"].dirListing = true;
	
    conf->config.locations["/folder/"].methodPost = false;
    conf->config.locations["/folder/"].methodGet = true;
    conf->config.locations["/folder/"].methodDelete = false;
    // conf->config.locations["/folder/"].dirListing = true;
	conf->config.locations["/folder/"].redirection = "/";
	conf->config.locations["/folder/"].defaultFile = "/errorHtml/500.html";
    conf->config.CGIDir = "";
	/* vector parsing structs */
	Server	server(conf->config);

	while (CPP_IS_PAIN)
	{
		server.serverLoop();
	}
	return 0;
}
