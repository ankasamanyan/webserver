#ifndef CONFIGURATIONFILEPARSER
#define CONFIGURATIONFILEPARSER
#include "Configuration.hpp"
#include "ConfigurationFileReader.hpp"


class ConfigurationFileParser {
    public:

	Configuration config;

    ConfigurationFileParser();
	~ConfigurationFileParser();

	// void ConfigurationFileParser::parseHost(){

	// }

	// void ConfigurationFileParser::parse(){
	// 	parseHost();
	// }

	/* TO IMPLEMENT
			
			parseServerName
			parseHost
			parsePort
			parsemaxBody
			parseError404
			parseRoot
			parseCGIDir
			parseDir
			parsePathToUploads
			parseHttpVersion
			parseDirListing
			parseGetAvailability
			parsePostAvailability
			parseDeletAvailability

			
*/

};

#endif


