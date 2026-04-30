#pragma once

#include <string>

enum class Protocol
{
	HTTP,
	HTTPS
};

class CHttpUrl
{
public:
	explicit CHttpUrl(const std::string& url);
	CHttpUrl(const std::string& domain, const std::string& document, Protocol protocol);
	CHttpUrl(const std::string& domain, const std::string& document, Protocol protocol, unsigned short port);

	[[nodiscard]] std::string GetURL() const;
	[[nodiscard]] std::string GetDomain() const noexcept;
	[[nodiscard]] std::string GetDocument() const noexcept;
	[[nodiscard]] Protocol GetProtocol() const noexcept;
	[[nodiscard]] std::string GetProtocolString() const noexcept;
	[[nodiscard]] unsigned short GetPort() const noexcept;

private:
	static void ValidateDomain(const std::string& domain);
	static void ValidateDocument(std::string& document);
	static void ValidatePort(unsigned short port);
	static Protocol ParseProtocol(const std::string& protocolStr);
	static unsigned GetDefaultPort(Protocol protocol) noexcept;
	static std::string GetProtocolString(Protocol protocol) noexcept;

	Protocol m_protocol;
	std::string m_domain;
	std::string m_document;
	unsigned short m_port;

	static constexpr unsigned HTTP_DEFAULT_PORT = 80;
	static constexpr unsigned HTTPS_DEFAULT_PORT = 443;
	static constexpr unsigned MIN_PORT = 1;
	static constexpr unsigned MAX_PORT = 65535;
};