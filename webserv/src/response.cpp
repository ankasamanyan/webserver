#include "../includes/Server.hpp"

void	Client::sendResponse()
{
	char			body[CHUNK_SIZE];
	string			fileName;

	/* <--------------- path -------------------> */
	/**
	 * @brief access path checks
	 * @return correct _exitstate
	 */


	if (_path.compare("/") == 0)
		fileName = "./html/index.html";
	else if (isMethodAllowed() == false) /*_exitState == ERROR_405 */
		fileName = "./html/405.html";
	else if(_exitState == ERROR_404)
		fileName = "./html/404.html";
	else
		fileName = "." + _path;
		/* with '.' for the docker */
	PRINT << GREEN "You are trying to access '" << _path << "'" << RESET_LINE<< RESET_LINE;
	std::ifstream	inputFile(fileName.c_str(), std::ios::binary); /* temp html file */

	/* check the _exitcide and the accessability beffore sending headers */

	if (_responseState == INITIALIZED)
		sendHeaders();
	if (inputFile.is_open())
	{
		// if(_responseState != FULLY_SENT)
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
		send(_clientFd, body, inputFile.gcount(), 0);

		/* change the temp position if the file was not read till the end */
		if (inputFile.eof())
		{
			PRINT << YELLOW "\t END OF FOLE" << RESET_LINE; 
			_responseState = FULLY_SENT;
			inputFile.close();
			return ;
		}
		_responsePos = inputFile.tellg();
		inputFile.close();
	}
	else 
	{
		/* if the file doesn't exist && _exitstate == 404 */
		_exitState = ERROR_404;
		fileName = "./html/404.html";
		std::ifstream errorPage(fileName.c_str(), std::ios::binary);
		errorPage.read(body, sizeof(body));
		send(_clientFd, body, errorPage.gcount(), 0);
		errorPage.close();
		_responseState = FULLY_SENT;
		return ;
		/*  */
		std::cout << PINK << "HALP, THERE IS PROBLEM WITH THE FILE " << RESET_LINE;
		std::cout << PINK << strerror(errno) << RESET_LINE;
	}
}

void	Client::sendHeaders()
{
	string			headers;
	string			response;

	response.append(HTTP_V); /* HTTP version */
	
	response.append(" 200 OK\r\n"); /* exit code */

	checkHeaders(headers);

	headers.append("\r\n");
	response.append(headers);

	PRINT << PURPLE << "The headers are:\t" << headers << RESET_LINE;

	send(_clientFd, response.c_str(), response.length(), 0);
	_responseState = PARTIALLY_SENT;
}

void	Client::checkHeaders(std::string &headers)
{
	std::string		ext;
	size_t 			ext_in = _path.rfind('.');

	if (ext_in != std::string::npos)
		ext = _path.substr(ext_in);
	if (ext.compare(".html") == 0)
		headers.append("content-type: text/html\r\n");
	else if (ext.compare(".htm") == 0)
		headers.append("content-type: text/html\r\n");
	else if (ext.compare(".css") == 0)
		headers.append("content-type: text/css\r\n");
	else if (ext.compare(".js") == 0)
		headers.append("content-type: application/javascript\r\n");
	else if (ext.compare(".json") == 0)
		headers.append("content-type: application/json\r\n");
	else if (ext.compare(".jpg") == 0)
		headers.append("content-type: image/jpeg\r\n");
	else if (ext.compare(".jpeg") == 0)
		headers.append("content-type: image/jpeg\r\n");
	else if (ext.compare(".png") == 0)
		headers.append("content-type: image/png\r\n");
	else if (ext.compare(".gif") == 0)
		headers.append("content-type: image/gif\r\n");
	else if (ext.compare(".bmp") == 0)
		headers.append("content-type: image/bmp\r\n");
	else if (ext.compare(".ico") == 0)
		headers.append("content-type: image/x-icon\r\n");
	else if (ext.compare(".svg") == 0)
		headers.append("content-type: image/svg+xml\r\n");
	else if (ext.compare(".xml") == 0)
		headers.append("content-type: application/xml\r\n");
	else if (ext.compare(".pdf") == 0)
		headers.append("content-type: application/pdf\r\n");
	else if (ext.compare(".zip") == 0)
		headers.append("content-type: application/zip\r\n");
	else if (ext.compare(".gz") == 0)
		headers.append("content-type: application/gzip\r\n");
	else if (ext.compare(".tar") == 0)
		headers.append("content-type: application/x-tar\r\n");
	else if (ext.compare(".mp4") == 0)
		headers.append("content-type: video/mp4\r\n");
	else if (ext.compare(".mpeg") == 0)
		headers.append("content-type: video/mpeg\r\n");
	else if (ext.compare(".avi") == 0)
		headers.append("content-type: video/x-msvideo\r\n");
	else if (ext.compare(".avif") == 0)
		headers.append("content-type: image/avif\r\n");
	else if (ext.compare(".ogg") == 0)
		headers.append("content-type: audio/ogg\r\n");
	else if (ext.compare(".mp3") == 0)
		headers.append("content-type: audio/mpeg\r\n");
	else if (ext.compare(".wav") == 0)
		headers.append("content-type: audio/wav\r\n");
	else if (ext.compare(".mov") == 0)
		headers.append("content-type: video/quicktime\r\n");
	else if (ext.compare(".ppt") == 0)
		headers.append("content-type: application/vnd.ms-powerpoint\r\n");
	else if (ext.compare(".xls") == 0)
		headers.append("content-type: application/vnd.ms-excel\r\n");
	else if (ext.compare(".doc") == 0)
		headers.append("content-type: application/msword\r\n");
	else if (ext.compare(".csv") == 0)
		headers.append("content-type: text/csv\r\n");
	else if (ext.compare(".txt") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".rtf") == 0)
		headers.append("content-type: application/rtf\r\n");
	else if (ext.compare(".shtml") == 0)
		headers.append("content-type: text/html\r\n");
	else if (ext.compare(".php") == 0)
		headers.append("content-type: application/php\r\n");
	else if (ext.compare(".jsp") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".swf") == 0)
		headers.append("content-type: application/x-shockwave-flash\r\n");
	else if (ext.compare(".ttf") == 0)
		headers.append("content-type: application/x-font-truetype\r\n");
	else if (ext.compare(".eot") == 0)
		headers.append("content-type: application/vnd.ms-fontobject\r\n");
	else if (ext.compare(".woff") == 0)
		headers.append("content-type: application/font-woff\r\n");
	else if (ext.compare(".woff2") == 0)
		headers.append("content-type: font/woff2\r\n");
	else if (ext.compare(".ics") == 0)
		headers.append("content-type: text/calendar\r\n");
	else if (ext.compare(".vcf") == 0)
		headers.append("content-type: text/x-vcard\r\n");
	else if (ext.compare(".mid") == 0)
		headers.append("content-type: audio/midi\r\n");
	else if (ext.compare(".midi") == 0)
		headers.append("content-type: audio/midi\r\n");
	else if (ext.compare(".wmv") == 0)
		headers.append("content-type: video/x-ms-wmv\r\n");
	else if (ext.compare(".webm") == 0)
		headers.append("content-type: video/webm\r\n");
	else if (ext.compare(".3gp") == 0)
		headers.append("content-type: video/3gpp\r\n");
	else if (ext.compare(".3g2") == 0)
		headers.append("content-type: video/3gpp2\r\n");
	else if (ext.compare(".pl") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".py") == 0)
		headers.append("content-type: text/x-python\r\n");
	else if (ext.compare(".java") == 0)
		headers.append("content-type: text/x-java-source\r\n");
	else if (ext.compare(".c") == 0)
		headers.append("content-type: text/x-c\r\n");
	else if (ext.compare(".cpp") == 0)
		headers.append("content-type: text/x-c++\r\n");
	else if (ext.compare(".cs") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".rb") == 0)
		headers.append("content-type: text/x-ruby\r\n");
	else if (ext.compare(".htm") == 0)
		headers.append("content-type: text/html\r\n");
	else if (ext.compare(".shtml") == 0)
		headers.append("content-type: text/html\r\n");
	else if (ext.compare(".xhtml") == 0)
		headers.append("content-type: application/xhtml+xml\r\n");
	else if (ext.compare(".m4a") == 0)
		headers.append("content-type: audio/mp4\r\n");
	else if (ext.compare(".mp4a") == 0)
		headers.append("content-type: audio/mp4\r\n");
	else if (ext.compare(".oga") == 0)
		headers.append("content-type: audio/ogg\r\n");
	else if (ext.compare(".ogv") == 0)
		headers.append("content-type: video/ogg\r\n");
	else if (ext.compare(".ogx") == 0)
		headers.append("content-type: application/ogg\r\n");
	else if (ext.compare(".oga") == 0)
		headers.append("content-type: audio/ogg\r\n");
	else if (ext.compare(".m3u8") == 0)
		headers.append("content-type: application/vnd.apple.mpegurl\r\n");
	else if (ext.compare(".qt") == 0)
		headers.append("content-type: video/quicktime\r\n");
	else if (ext.compare(".ts") == 0)
		headers.append("content-type: video/mp2t\r\n");
	else if (ext.compare(".xl") == 0)
		headers.append("content-type: application/excel\r\n");
	else if (ext.compare(".cab") == 0)
		headers.append("content-type: application/vnd.ms-cab-compressed\r\n");
	else if (ext.compare(".msi") == 0)
		headers.append("content-type: application/x-msdownload\r\n");
	else if (ext.compare(".dmg") == 0)
		headers.append("content-type: application/x-apple-diskimage\r\n");
	else if (ext.compare(".exe") == 0)
		headers.append("content-type: application/octet-stream\r\n");
	else if (ext.compare(".bin") == 0)
		headers.append("content-type: application/octet-stream\r\n");
	else if (ext.compare(".ps") == 0)
		headers.append("content-type: application/postscript\r\n");
	else if (ext.compare(".so") == 0)
		headers.append("content-type: application/octet-stream\r\n");
	else if (ext.compare(".dll") == 0)
		headers.append("content-type: application/octet-stream\r\n");
	else if (ext.compare(".m4v") == 0)
		headers.append("content-type: video/x-m4v\r\n");
	else if (ext.compare(".ser") == 0)
		headers.append("content-type: application/java-serialized-object\r\n");
	else if (ext.compare(".sh") == 0)
		headers.append("content-type: application/x-sh\r\n");
	else if (ext.compare(".log") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".diff") == 0)
		headers.append("content-type: text/x-diff\r\n");
	else if (ext.compare(".patch") == 0)
		headers.append("content-type: text/x-diff\r\n");
	else if (ext.compare(".xhtml") == 0)
		headers.append("content-type: application/xhtml+xml\r\n");
	else if (ext.compare(".php") == 0)
		headers.append("content-type: application/x-httpd-php\r\n");
	else if (ext.compare(".plist") == 0)
		headers.append("content-type: application/xml\r\n");
	else if (ext.compare(".sln") == 0)
		headers.append("content-type: text/plain\r\n");
	else if (ext.compare(".tiff") == 0)
		headers.append("content-type: image/tiff\r\n");
	else if (ext.compare(".app") == 0)
		headers.append("content-type: application/octet-stream\r\n");
	else if (ext.compare(".ics") == 0)
		headers.append("content-type: text/calendar\r\n");
}