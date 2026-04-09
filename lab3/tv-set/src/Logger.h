#pragma once
#include <iostream>
#include <string>

class Logger
{
public:
	static void Debug(const std::string& msg)
	{
		std::cerr << "[DEBUG] " << msg << "\n";
	}

	static void Error(std::ostream& output, const std::string& publicMsg)
	{
		Debug("Error: " + publicMsg);
		output << "ERROR\n";
	}
};