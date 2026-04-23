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

	if (!date.IsValid())
	{
		std::cout << "Date is invalid\n";
		return 0;
	}

	std::cout << "Parsed: " << date << '\n';
	std::cout << "Day: " << static_cast<int>(date.GetDay()) << '\n';
	std::cout << "Month: " << static_cast<int>(date.GetMonth()) << '\n';
	std::cout << "Year: " << static_cast<int>(date.GetYear()) << '\n';

	std::cout << date << '\n';
	return 0;
}