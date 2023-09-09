#include "../includes/Server.hpp"

void	Client::sendResponse()
{
	char			body[CHUNK_SIZE];
	string			fileName;

	if (_exitState == EXIT_OK)
		fileName = "." + _requestTarget;
	else
		fileName.append(".").append(_configuration.root).append(_errorPagePath).append(std::to_string((int)_exitState)).append(".html");
	PRINT << PINK "The code: x"<< "\t" << YELLOW "The state: "<< _exitState << RESET_LINE;
	PRINT << YELLOW "File Name: "<< fileName <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;
	// _exitState = EXIT_OK;
	/* if it's a get request, check if it's a dir_listing case */
	// putCorrectPath(fileName);

	PRINT << GREEN "You are trying to access '" << fileName << "'" << RESET_LINE<< RESET_LINE;
	std::ifstream	inputFile(fileName.c_str(), std::ios::binary); /* temp html file */

	/* check the _exitcide and the accessability beffore sending headers */

	if (_responseState == INITIALIZED)
		sendHeaders();
	if (inputFile.is_open())
	{
		// if(_responseState != FULLY_SENT)
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
		if (_responseState == INITIALIZED)
			sendHeaders();
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
	else if (!inputFile) 
	{
		/* if the file doesn't exist && _exitstate == 404 */
		_exitState = ERROR_404;
		fileName = STANDARD_404;
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
	string			errorCode;

	response.append(HTTP_V); /* HTTP version */
	errorCode.append(" ").append(std::to_string((int)_exitState)).append(" ").append(getHttpMsg((int)_exitState));
	response.append(errorCode).append("\r\n");
	// response.append(" 200 OK\r\n"); /* exit code */
	PRINT << PURPLE << "ERROR CODES:\t" << errorCode << RESET_LINE;
	checkHeaders(headers);
	headers.append("\r\n");
	response.append(headers);
	// PRINT << PURPLE << "The headers are:\t" << headers << RESET_LINE;
	PRINT << PURPLE << "Response:\n" << response << RESET_LINE;

	send(_clientFd, response.c_str(), response.length(), 0);
	_responseState = PARTIALLY_SENT;

}
