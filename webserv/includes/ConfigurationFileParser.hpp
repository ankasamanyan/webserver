#ifndef CONFIGURATIONFILEPARSER
#define CONFIGURATIONFILEPARSER
#include "Configuration.hpp"
#include "ConfigurationFileReader.hpp"


class ConfigurationFileParser {
    public:

	ConfigurationFileParser();
    ConfigurationFileParser(Configuration config);
	~ConfigurationFileParser();

	// void ConfigurationFileParser::parseHost();
	void parse();

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


