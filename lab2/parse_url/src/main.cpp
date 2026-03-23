#include "parse_url.h"
#include <iostream>
#include <string>
// TODO: разобраться с форматированием
int main()
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
			continue;

		UrlInfo info;
		if (ParseUrl(line, info))
		{
			PrintUrlInfo(line, info);
		}
		else
		{
			std::cout << "ERROR\n";
			return 1;
		}
	}
	return 0;
}
