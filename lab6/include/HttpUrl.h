#pragma once

#include <string>

enum class Protocol
{
	HTTP,
	HTTPS
};

class HttpUrl
{
public:
	// explicit обязателен, чтобы запрещать неявное преобразование string -> CHttpUrl
	explicit HttpUrl(const std::string& url);

	HttpUrl(const std::string& domain, const std::string& document, Protocol protocol = Protocol::HTTP);
	HttpUrl(const std::string& domain, const std::string& document, Protocol protocol, unsigned short port);

	// [[nodiscard]] запрещает компилятору игнорировать возвращаемое значение без явного cast в void
	// Для геттеров это полезно: если вызвали GetURL() и не использовали результат — скорее всего, это баг
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
	static unsigned short GetDefaultPort(Protocol protocol) noexcept;
	static std::string GetProtocolString(Protocol protocol) noexcept;
	static unsigned short ParsePortNumber(const std::string& portStr);

	Protocol m_protocol;
	std::string m_domain;
	std::string m_document;

	unsigned short m_port;

	static constexpr unsigned short HTTP_DEFAULT_PORT = 80;
	static constexpr unsigned short HTTPS_DEFAULT_PORT = 443;
	static constexpr unsigned short MIN_PORT = 1;
	static constexpr unsigned short MAX_PORT = 65535;
};