#include "include/CDate.h"
#include "include/Logger/Logger.h"
#include <iostream>
int main()
{
	LoggerInit(LogType::Debug);

	CDate date(1, Month::January, 2024);
	std::cout << "Start: " << date << '\n';

	date += 60;
	std::cout << "+60 days: " << date << '\n';

	CDate other = date - 30;
	std::cout << "-30 days: " << other << '\n';
	std::cout << "Diff: " << (date - other) << " days\n";

	return 0;
}