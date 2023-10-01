#ifndef	UTILS_HPP
	#define UTILS_HPP

#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <vector>

# define CPP_IS_PAIN	1
# define RESET			"\033[0m"
# define BLACK			"\033[0;30m"
# define RED			"\033[0;31m"
# define GREEN			"\033[0;32m"
# define YELLOW			"\033[0;33m"
# define PURPLE			"\033[0;34m"
# define PINK			"\033[0;35m"
# define SKY			"\033[0;36m"
# define BLUE			"\033[38;5;81m"
# define ORANGE			"\033[38;2;255;165;0m"
# define LILY			"\033[38;5;219m"
# define GRAY			"\033[38;5;8m"
# define ON_RED 		"\033[41m"
# define ON_PURPLE		"\033[44m"
# define ON_PINK		"\033[45m"
# define RESET_LINE 	RESET << std::endl
# define PRINT			std::cout
# define RE_TERMINAL	std::cout << "\e[1;1H\e[2J"

class Utils
{
	public:
		static void	printMsg(std::string stringy, std::string colour)
		{
			std::cout << std::endl << colour << "+" << std::string(49, '=') << "+" << RESET_LINE;
			std::cout << RESET << (stringy.size() > 16 ? "	" : "		");
			std::cout<< "..." << stringy << "..." << RESET_LINE;
			std::cout << colour << "+" << std::string(49, '=') << "+" << std::endl << RESET_LINE;
		}

		static bool strCheck(std::string stringy, int(*func)(int c))
		{
			for (std::string::const_iterator it = stringy.begin(); it != stringy.end(); ++it)
			{
				if (!(*func)(*it))
					return(false);
			}
			return(true);
		}

		static int findChar(std::string input, char c)
		{
			int amount = 0;
			for (std::string::const_iterator it = input.begin(); it != input.end(); ++it)
		   		if (*it == c)
		      		amount++;
			return (amount);
		}

		static int noGarbageInDouble(int c)
		{
			if (!isdigit(c) && c != '.')
				return (false);
			return (true);
		}

		static bool	detectDouble(std::string input)
		{
			if(input[0] == '-' || input[0] == '+')
				input = input.substr(1);
			if (findChar(input,'.') == 1 && strCheck(input, noGarbageInDouble))
			{
				if (input[0] != '.' && input[input.size() - 1] != '.')
					return (true);
			}
			return (false);
		}
		bool detectInt(std::string input)
		{
			if(input[0] == '-' || input[0] == '+')
				input = input.substr(1);
			if (strCheck(input, isdigit))
				return (true);
			return (false);
		}
        static std::vector<std::string> split(std::string string, const char separator) {
            std::vector<std::string>	splitString;
            std::stringstream			streamString(string);
            std::string					partOfString;

            while(std::getline(streamString, partOfString, separator)) {
                if(partOfString.length() > 0)
                    splitString.push_back(partOfString);
            }
            return splitString;
        }
        static std::string trimLeft(std::string string, const char *sequenceToErase) {
            string.erase(0, string.find_first_not_of(sequenceToErase));
            return string;
        }
        static std::string trimRight(std::string string, const char *sequenceToErase) {
            string.erase(string.find_last_not_of(sequenceToErase) + 1);
            return string;
        }
		static int	ft_intlen(int n)
		{
			int	i;

			i = 0;
			if (n == 0)
				i++;
			if (n < 0)
			{
				n *= -1;
				i++;
			}
			while (n > 0)
			{
				i++;
				n /= 10;
			}
			return (i);
		}
		static void	ft_itoa(int n, std::string &stringy)
		{
			int		i = 0;
			int		len = ft_intlen(n);;
			stringy.reserve(len);

			if (n < 0)
			{
				printMsg("Your itoa is unfortunately not working ", RED);
				return ;
			}
			while (i < len--)
			{
				stringy.insert(0, 1, (n%10+'0'));
				n /= 10;
			}
		}
};

#define HTML_HEADERS "HTTP/1.1 200 OK\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Directory Listing</title>"

#define STYLES "<style> body { background-image: #6c55bea9;background-repeat: repeat;background-size: 205px;background-position: 0 50px; \
font-family: Arial, sans-serif;margin: 0;padding: 0;display: flex;flex-direction: column;justify-content: center; \
align-items: center;min-height: 100vh;animation: backgroundColorChange 5s linear infinite;} \
.topbar {color: #6c55be;padding: 30px 0;font-size: 23px;display: flex;justify-content: space-between;align-items: center; \
position: fixed;top: 0;width: 100%;z-index: 999;} \
.topbar a {color: #ffffff;text-decoration: none;margin: 0 30px;} \
.company-name {color: #ffffff;margin: 0 30px;} \
.content {padding: 80px;border-radius: 15px;text-align: center;display: flex;flex-wrap: wrap;justify-content: center;} \
.item {background-color: #f4f4f438;padding: 20px;margin: 10px; border-radius: 10px;text-align: center;width: 188px; \
min-height: 180px;display: flex;flex-direction: column;justify-content: center;align-items: center;} \
.folder {color: #ffffff;font-size: 24px;}.folder a {text-decoration: none}.file {color: #ffffff;font-size: 24px;}.file a {text-decoration: none}.icon {font-size: 72px;margin-bottom: 20px;} \
@keyframes backgroundColorChange {0% {background-color: #6c55bea9;}25% {background-color: #9f53b0a9;} \
50% {background-color: #c87b40a9;}75% {background-color: #b05374a9;}100% {background-color: #6c55bea9;}}</style>"

#define BODY "<body><div class=\"topbar\"><div class=\"company-name\"><a href=\"/\">✨42Capybaras✨</a></div> \
<div class=\"links\"><a href=\"../Uploads.html\">Upload</a><a href=\"../cgi-form.html\">CGI</a><a href=\"../Contact-us.html\">Contact us</a> \
</div></div><div class=\"content\">"

#define CLOSING_TAGS "</div></body></html>"

#define FOLDER_FORM_OPEN "<div class=\"item folder\"><div class=\"icon\">📁</div><div class=\"folder\">"

#define FILE_FORM_OPEN "<div class=\"item file\"><div class=\"icon\">📄</div> <div class=\"file\">"

#define CLOSING_DIVS "</div></div>"

#endif

//256
// Foreground: \033[38;5;<color_number>m
// Background: \033[48;5;<color_number>m
//rgb
// Foreground: \033[38;2;<r>;<g>;<b>m
// Background: \033[48;2;<r>;<g>;<b>m
