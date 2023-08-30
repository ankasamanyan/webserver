#ifndef	UTILS_HPP
	#define UTILS_HPP

#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>

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
};

#endif

//256
// Foreground: \033[38;5;<color_number>m
// Background: \033[48;5;<color_number>m
//rgb
// Foreground: \033[38;2;<r>;<g>;<b>m
// Background: \033[48;2;<r>;<g>;<b>m
