#include "Calculator.h"
#include "Logger.h"
#include "Parser.h"
#include <iostream>
#include <string>

int main()
{
	Logger::Info("Calculator started");

	Calculator calculator;
	Parser parser(calculator);

	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			continue;
		}
		parser.Parse(line);
	}

	Logger::Info("Calculator finished");
	return 0;
}