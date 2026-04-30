#pragma once

#include <stdexcept>
#include <string>

class UrlParsingError : public std::invalid_argument
{
public:
	explicit UrlParsingError(const std::string& message)
		: std::invalid_argument(message)
	{
	}
};
