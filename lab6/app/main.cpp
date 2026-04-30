#include "HttpUrl.h"
#include "UrlParsingError.h"
#include <iostream>
#include <string>

int main()
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			continue;
		}

		try
		{
			CHttpUrl url(line);
			std::clog << "[INFO] Parsed URL: " << line << "\n";
			std::cout << "Full URL: " << url.GetURL() << "\n";
			std::cout << "Protocol: " << url.GetProtocolString() << "\n";
			std::cout << "Domain: " << url.GetDomain() << "\n";
			std::cout << "Port: " << url.GetPort() << "\n";
			std::cout << "Document: " << url.GetDocument() << "\n";
		}
		catch (const UrlParsingError& e)
		{
			std::cerr << "[ERROR] URL parsing failed: " << e.what() << "\n";
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << "[ERROR] Invalid argument: " << e.what() << "\n";
		}
		catch (const std::exception& e)
		{
			std::cerr << "[ERROR] Unexpected error: " << e.what() << "\n";
		}
	}
	return 0;
}