#include "../includes/Server.hpp"

void	Client::sendResponse()
{
	char			body[CHUNK_SIZE];
	string			fileName;
	char			str[42];

	Utils::ft_itoa(((int)_exitState), str);
	if (_exitState == EXIT_OK)
	{
		if (_path.compare("/") == 0)
			fileName = STANDARD_HTML;
		else
			fileName = "." + _requestTarget;
	}
	else
	{
		fileName.append(".").append(_configuration.root).append(_errorPagePath);
		fileName.append(str).append(".html");
	}
	PRINT << PINK "The code: "<< "\t" << str << YELLOW "The state: "<< _exitState << RESET_LINE;
	PRINT << YELLOW "File Name: "<< fileName <<"\t"<< PINK "Request tartrget: "<< _requestTarget << RESET_LINE;

	PRINT << GREEN "You are trying to access '" << fileName << "'" << RESET_LINE<< RESET_LINE;

	std::ifstream	inputFile(fileName.c_str(), std::ios::binary); /* temp html file */
	
	if (inputFile.is_open())
	{
		inputFile.seekg(_responsePos);
		inputFile.read(body, sizeof(body));
		if (_responseState == INITIALIZED && exists(fileName))
			sendHeaders();
		send(_clientFd, body, inputFile.gcount(), 0);

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
		_exitState = ERROR_404;
		if (_responseState == INITIALIZED)
			sendHeaders();
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
	char			str[42];

	Utils::ft_itoa((int)_exitState, str);
	response.append(HTTP_V);
	errorCode.append(" ").append(str).append(" ").append(getHttpMsg((int)_exitState));
	response.append(errorCode).append("\r\n");
	PRINT << PURPLE << "ERROR CODES:\t" << str << RESET_LINE;
	if (_exitState != ERROR_404)
		checkHeaders(headers);
	headers.append("\r\n");
	response.append(headers);
	PRINT << PURPLE << "Response:\n" << response << RESET_LINE;
	send(_clientFd, response.c_str(), response.length(), 0);
	_responseState = PARTIALLY_SENT;

}
