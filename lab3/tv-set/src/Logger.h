#pragma once
#include <iostream>
#include <string>

class Logger
{
public:
	static void Debug(const std::string& msg)
	{
#ifdef DEBUG_LOG
		std::cerr << "[DEBUG] " << msg << "\n";
#endif
	}

	static void Error(std::ostream& output, const std::string& publicMsg)
	{
		Debug("Error: " + publicMsg);
		output << "ERROR\n";
	}
};