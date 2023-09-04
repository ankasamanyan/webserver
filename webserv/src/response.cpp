#include "../includes/Server.hpp"

void	Server::sendResponse(fdIter iter)
{
	// string			header;
	string			body;
	// char			body[CHUNK_SIZE];
	string			line;
	string			response;
	std::ifstream	inputFile("text.html");
	std::streamsize	chunk(CHUNK_SIZE);

	response.append(HTTP_V);
	response.append(" 200 OK\r\n\r\n");

	if (inputFile.is_open())
	{
		// inputFile.read(body, sizeof(body));
		
		while (std::getline(inputFile, line) && body.length() < CHUNK_SIZE) 
			body.append(line);
		inputFile.close();
	}
	else 
	{
		std::cout << PINK << "HALP, THERE IS PROBLEM WITH THE FILE " << RESET_LINE;
	}
	// PRINT << GRAY << body << RESET_LINE;
	response.append(body);
	send(iter->fd, response.c_str(), response.length(), 1);
}