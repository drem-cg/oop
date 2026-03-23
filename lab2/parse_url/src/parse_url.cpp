#include "parse_url.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <regex>
#include <utility>

constexpr auto kUrlPattern = R"(^([a-zA-Z]+)://([^:/\s]+)(?::(\d+))?(?:/(.*))?$)";

constexpr std::array<std::pair<const char*, Protocol>, 3> kProtocolMap = {
	{ { "http", HTTP },
		{ "https", HTTPS },
		{ "ftp", FTP } }
};

Protocol StringToProtocol(const std::string& protocol_string)
{
	std::string lowerCaseProtocol = protocol_string;
	std::transform(lowerCaseProtocol.begin(), lowerCaseProtocol.end(), lowerCaseProtocol.begin(),
		// TODO: virtual_cast и разница со static_cast
		[](const unsigned char c) { return static_cast<char>(std::tolower(c)); });

	for (const auto& [name, proto] : kProtocolMap)
	{
		if (lowerCaseProtocol == name)
		{
			return proto;
		}
	}
	return UNKNOWN;
}

int GetDefaultPort(const Protocol protocol)
{
	switch (protocol)
	{
	case HTTP:
		return 80;
	case HTTPS:
		return 443;
	case FTP:
		return 21;
	default:
		return 0;
	}
}

bool ParsePort(const std::string& port_str, int& port_int)
{
	if (port_str.empty())
	{
		return false;
	}

	try
	{
		// TODO: исключения stoi
		const int value = std::stoi(port_str);
		if (value < kMinPort || value > kMaxPort)
		{
			return false;
		}
		port_int = value;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool ParseUrl(const std::string& url, UrlInfo& info)
{
	static const std::regex regexUrl(kUrlPattern, std::regex_constants::icase);
	std::smatch match;

	if (!std::regex_match(url, match, regexUrl))
	{
		return false;
	}

	const std::string protocol = match[1].str();
	info.protocol = StringToProtocol(protocol);
	if (info.protocol == UNKNOWN)
	{
		return false;
	}

	info.host = match[2].str();
	if (info.host.empty())
	{
		return false;
	}

	if (match[3].matched)
	{
		if (!ParsePort(match[3].str(), info.port))
		{
			return false;
		}
	}
	else
	{
		info.port = GetDefaultPort(info.protocol);
	}

	info.document = match[4].matched ? match[4].str() : "";

	return true;
}

void PrintUrlInfo(const std::string& original_url, const UrlInfo& info)
{
	std::cout << original_url << '\n';
	std::cout << "HOST: " << info.host << '\n';
	std::cout << "PORT: " << info.port << '\n';
	std::cout << "DOC: " << info.document << '\n';
}
