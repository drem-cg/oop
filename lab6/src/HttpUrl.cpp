#include "HttpUrl.h"
#include "UrlParsingError.h"

#include <algorithm>
#include <cctype>

namespace
{
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

struct UrlParts
{
	std::string protocol;
	std::string hostAndPort;
	std::string document;
};

UrlParts SplitUrlIntoParts(const std::string& url)
{
	const auto protocolEnd = url.find("://");
	if (protocolEnd == std::string::npos)
	{
		throw UrlParsingError("Missing '://' in URL");
	}

	UrlParts parts;
	parts.protocol = url.substr(0, protocolEnd);

	const auto pathStart = url.find('/', protocolEnd + 3);
	parts.hostAndPort = url.substr(protocolEnd + 3,
		(pathStart == std::string::npos) ? std::string::npos : pathStart - (protocolEnd + 3));

	if (pathStart == std::string::npos)
	{
		parts.document = "/";
	}
	else
	{
		parts.document = url.substr(pathStart);
	}

	return parts;
}

struct UrlStrParts
{
	std::string domain;
	std::string portStr;
	bool hasPort;
};

UrlStrParts SplitUrlStrParts(const std::string& str)
{
	const auto portSep = str.find(':');
	if (portSep == std::string::npos)
	{
		return { str, "", false };
	}

	const std::string portStr = str.substr(portSep + 1);
	if (portStr.empty())
	{
		throw UrlParsingError("Port number is missing after ':'");
	}

	return { str.substr(0, portSep), portStr, true };
}
} // namespace

CHttpUrl::CHttpUrl(const std::string& domain, const std::string& document, const Protocol protocol)
	: CHttpUrl(domain, document, protocol, GetDefaultPort(protocol))
{
}

CHttpUrl::CHttpUrl(const std::string& domain, const std::string& document, const Protocol protocol, const unsigned port)
	: m_protocol(protocol)
	, m_domain(domain)
	, m_document(document)
	, m_port(port)
{
	ValidateDomain(m_domain);
	ValidateDocument(m_document);
	ValidatePort(m_port);
}

CHttpUrl::CHttpUrl(const std::string& url)
{
	const auto [protocol, hostAndPort, document] = SplitUrlIntoParts(url);
	m_protocol = ParseProtocol(protocol);

	const auto [domain, portStr, hasPort] = SplitUrlStrParts(hostAndPort);
	m_domain = domain;
	ValidateDomain(m_domain);

	if (hasPort)
	{
		m_port = ParsePortNumber(portStr);
	}
	else
	{
		m_port = GetDefaultPort(m_protocol);
	}

	m_document = document;
	ValidateDocument(m_document);
}

unsigned CHttpUrl::ParsePortNumber(const std::string& portStr)
{
	try
	{
		const unsigned portNum = std::stoul(portStr);
		if (portNum < MIN_PORT || portNum > MAX_PORT)
		{
			throw UrlParsingError("Port out of valid range from 1 to 65535");
		}
		return portNum;
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

void CHttpUrl::ValidateDomain(const std::string& domain)
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

void CHttpUrl::ValidateDocument(std::string& document)
{
	EnsureLeadingSlash(document);
}

void CHttpUrl::ValidatePort(const unsigned port)
{
	if (port < MIN_PORT || port > MAX_PORT)
	{
		throw UrlParsingError("Port out of valid range from 1 to 65535");
	}
}

Protocol CHttpUrl::ParseProtocol(const std::string& protocolStr)
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

unsigned CHttpUrl::GetDefaultPort(const Protocol protocol) noexcept
{
	return (protocol == Protocol::HTTP) ? HTTP_DEFAULT_PORT : HTTPS_DEFAULT_PORT;
}

std::string CHttpUrl::GetProtocolString(const Protocol protocol) noexcept
{
	return (protocol == Protocol::HTTP) ? "http" : "https";
}

std::string CHttpUrl::GetURL() const
{
	std::string result = GetProtocolString(m_protocol) + "://" + m_domain;

	if (m_port != GetDefaultPort(m_protocol))
	{
		result += ':' + std::to_string(m_port);
	}

	result += m_document;
	return result;
}

std::string CHttpUrl::GetDomain() const noexcept
{
	return m_domain;
}

std::string CHttpUrl::GetDocument() const noexcept
{
	return m_document;
}

Protocol CHttpUrl::GetProtocol() const noexcept
{
	return m_protocol;
}

std::string CHttpUrl::GetProtocolString() const noexcept
{
	return (m_protocol == Protocol::HTTP) ? "http" : "https";
}

unsigned CHttpUrl::GetPort() const noexcept
{
	return m_port;
}