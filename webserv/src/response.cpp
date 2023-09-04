#include "../includes/Server.hpp"

void	Server::sendResponse(fdIter iter)
{
	char			body[CHUNK_SIZE];
	string			response;
	std::ifstream	inputFile("text.html", std::ios::binary);
	std::streamsize	chunk(CHUNK_SIZE);

	response.append(HTTP_V); /* HTTP version */
	response.append(" 200 OK\r\n\r\n"); /* exit code */
	send(iter->fd, response.c_str(), response.length(), 0);

	if (inputFile.is_open())
	{
		// inputFile.seekg(/* temp position */);
		inputFile.read(body, sizeof(body));
		/* change the temp position if the file was not read till the end */
		inputFile.close();
	}
	else 
	{
		std::cout << PINK << "HALP, THERE IS PROBLEM WITH THE FILE " << RESET_LINE;
	}
	send(iter->fd, body, inputFile.gcount(), 0);
}