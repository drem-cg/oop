#include "HttpUrl.h"
#include "UrlParsingError.h"

#include <algorithm>
#include <cctype>
#include <regex>

namespace
{
constexpr auto kUrlPattern = R"(^([a-zA-Z]+)://([^:/\s]+)(?::(\d+))?(?:/(.*))?$)";

void ToLowerInPlace(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](const unsigned char c) { return std::tolower(c); });
}

void EnsureLeadingSlash(std::string& document)
{
	if (document.empty() || document[0] != '/')
	{
		document.insert(document.begin(), '/');
	}
}

// FIXED: разделил хост и порт
struct ParsedUrl
{
	std::string protocol;
	std::string host;
	std::string portStr;
	std::string document;
};


// FIXED: переделал под regix
ParsedUrl ParseUrlWithRegex(const std::string& url)
{
	static const std::regex urlRegex(kUrlPattern, std::regex::icase);
	std::smatch match;

	if (!std::regex_match(url, match, urlRegex))
	{
		throw UrlParsingError("Invalid URL format");
	}

	ParsedUrl result;
	result.protocol = match[1].str();
	result.host = match[2].str();
	result.portStr = match[3].str();
	result.document = match[4].str();

	if (result.document.empty())
	{
		result.document = "/";
	}

	return result;
}

} // namespace

void HttpUrl::ValidateDomain(const std::string& domain)
{
	if (domain.empty())
	{
		throw UrlParsingError("Domain cannot be empty");
	}
	if (domain.find_first_of(" \t\n\r") != std::string::npos)
	{
		throw UrlParsingError("Domain contains invalid characters");
	}
}

void HttpUrl::ValidateDocument(std::string& document)
{
	EnsureLeadingSlash(document);
}

void HttpUrl::ValidatePort(const unsigned short port)
{
	if (port < MIN_PORT || port > MAX_PORT)
	{
		throw UrlParsingError("Port out of valid range [1; 65535]");
	}
}

Protocol HttpUrl::ParseProtocol(const std::string& protocolStr)
{
	std::string lower = protocolStr;
	ToLowerInPlace(lower);

	if (lower == "http")
	{
		return Protocol::HTTP;
	}
	if (lower == "https")
	{
		return Protocol::HTTPS;
	}
	throw UrlParsingError("Unsupported protocol: '" + protocolStr + "'");
}

unsigned short HttpUrl::GetDefaultPort(const Protocol protocol) noexcept
{
	return (protocol == Protocol::HTTP) ? HTTP_DEFAULT_PORT : HTTPS_DEFAULT_PORT;
}

std::string HttpUrl::GetProtocolString(const Protocol protocol) noexcept
{
	return (protocol == Protocol::HTTP) ? "http" : "https";
}

unsigned short HttpUrl::ParsePortNumber(const std::string& portStr)
{
	try
	{
		const unsigned long portNum = std::stoul(portStr);
		if (portNum < MIN_PORT || portNum > MAX_PORT)
		{
			throw UrlParsingError("Port out of valid range [1; 65535]");
		}
		return static_cast<unsigned short>(portNum);
	}
	catch (const std::invalid_argument&)
	{
		throw UrlParsingError("Invalid port number format");
	}
	catch (const std::out_of_range&)
	{
		throw UrlParsingError("Port number is too large");
	}
}

HttpUrl::HttpUrl(const std::string& domain, const std::string& document, const Protocol protocol)
	: HttpUrl(domain, document, protocol, GetDefaultPort(protocol))
{
}

HttpUrl::HttpUrl(
	const std::string& domain,
	const std::string& document,
	const Protocol protocol,
	const unsigned short port)
	: m_protocol(protocol)
	, m_domain(domain)
	, m_document(document)
	, m_port(port)
{
	try
	{
		ValidateDomain(m_domain);
		ValidateDocument(m_document);
		ValidatePort(m_port);
	}
	catch (const UrlParsingError& e)
	{
		throw std::invalid_argument(e.what());
	}
}

HttpUrl::HttpUrl(const std::string& url)
{
	const ParsedUrl parsed = ParseUrlWithRegex(url);

	m_protocol = ParseProtocol(parsed.protocol);
	m_domain = parsed.host;
	ValidateDomain(m_domain);

	if (!parsed.portStr.empty())
	{
		m_port = ParsePortNumber(parsed.portStr);
	}
	else
	{
		m_port = GetDefaultPort(m_protocol);
	}

	m_document = parsed.document;
	ValidateDocument(m_document);
}

std::string HttpUrl::GetURL() const
{
	std::string result = GetProtocolString(m_protocol) + "://" + m_domain;

	if (m_port != GetDefaultPort(m_protocol))
	{
		result += ':' + std::to_string(m_port);
	}

	result += m_document;
	return result;
}

std::string HttpUrl::GetDomain() const noexcept
{
	return m_domain;
}

std::string HttpUrl::GetDocument() const noexcept
{
	return m_document;
}

Protocol HttpUrl::GetProtocol() const noexcept
{
	return m_protocol;
}

std::string HttpUrl::GetProtocolString() const noexcept
{
	return (m_protocol == Protocol::HTTP) ? "http" : "https";
}

unsigned short HttpUrl::GetPort() const noexcept
{
	return m_port;
}