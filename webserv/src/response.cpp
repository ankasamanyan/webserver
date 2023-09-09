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
//		fileName = "." + _path;
		fileName = "." + _requestTarget;
		/* with '.' for the docker */
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
