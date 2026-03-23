#pragma once

#include <string>

enum Protocol
{
	HTTP,
	HTTPS,
	FTP,
	UNKNOWN
};

struct UrlInfo
{
	Protocol protocol;
	int port;
	std::string host;
	std::string document;
};
// TODO: название констант поправить
constexpr long long kMinPort = 1;
constexpr long long kMaxPort = 65535;

bool ParseUrl(const std::string& url, UrlInfo& info);

void PrintUrlInfo(const std::string& original_url, const UrlInfo& info);
