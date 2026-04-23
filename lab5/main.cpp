#include "../lab4/src/Logger/Logger.h"
#include "CDate.h"

#include <iostream>

int main()
{
	LoggerInit(LogType::Debug);
	CDate date;
	std::cin >> date;

	if (std::cin.fail())
	{
		return 1;
	}

	std::cout << date << '\n';
	return 0;
}