#include "../include/HttpUrl.h"
#include "../include/UrlParsingError.h"
#include <gtest/gtest.h>

TEST(HttpUrlParser, ValidUrlsFromString)
{
	HttpUrl url1("http://test.com");
	EXPECT_EQ(url1.GetProtocol(), Protocol::HTTP);
	EXPECT_EQ(url1.GetDomain(), "test.com");
	EXPECT_EQ(url1.GetPort(), 80);
	EXPECT_EQ(url1.GetDocument(), "/");
	EXPECT_EQ(url1.GetURL(), "http://test.com/");

	HttpUrl url2("HTTPS://test.COM:443/path");
	EXPECT_EQ(url2.GetProtocol(), Protocol::HTTPS);
	EXPECT_EQ(url2.GetDomain(), "test.COM");
	EXPECT_EQ(url2.GetPort(), 443);
	EXPECT_EQ(url2.GetDocument(), "/path");
	EXPECT_EQ(url2.GetURL(), "https://test.COM/path");

	HttpUrl url3("http://host.org:8080/index.html");
	EXPECT_EQ(url3.GetPort(), 8080);
	EXPECT_EQ(url3.GetURL(), "http://host.org:8080/index.html");
}

TEST(HttpUrlParser, InvalidUrlsFromString)
{
	ASSERT_THROW(HttpUrl("ftp://test.com"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http//test.com"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://:80/path"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://domain:abc"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://domain:99999"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://domain:0/path"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://test .com"), UrlParsingError);
}

TEST(HttpUrlComponents, ValidConstruction)
{
	const HttpUrl url1("test.com", "index.html", Protocol::HTTP);
	EXPECT_EQ(url1.GetDocument(), "/index.html");
	EXPECT_EQ(url1.GetPort(), 80);

	const HttpUrl url2("test.com", "", Protocol::HTTPS, 8443);
	EXPECT_EQ(url2.GetDocument(), "/");
	EXPECT_EQ(url2.GetPort(), 8443);
}

TEST(HttpUrlComponents, InvalidConstruction)
{
	ASSERT_THROW(HttpUrl("", "/doc", Protocol::HTTP), std::invalid_argument);
	ASSERT_THROW(HttpUrl("domain", "/doc", Protocol::HTTP, 0), std::invalid_argument);
	ASSERT_THROW(HttpUrl("domain", "/doc", Protocol::HTTP, 65536), std::invalid_argument);
}

TEST(HttpUrlBoundary, PortLimitsAndNormalization)
{
	HttpUrl minPort("http://a.com:1");
	EXPECT_EQ(minPort.GetPort(), 1);

	HttpUrl maxPort("http://a.com:65535");
	EXPECT_EQ(maxPort.GetPort(), 65535);

	HttpUrl emptyDoc("http://a.com");
	EXPECT_EQ(emptyDoc.GetDocument(), "/");

	HttpUrl noSlashDoc("http://a.com", "page.html", Protocol::HTTP);
	EXPECT_EQ(noSlashDoc.GetDocument(), "/page.html");

	HttpUrl alreadySlashDoc("http://a.com", "/doc", Protocol::HTTP);
	EXPECT_EQ(alreadySlashDoc.GetDocument(), "/doc");
}

TEST(HttpUrlParser, DefaultProtocolIsHttp)
{
	const HttpUrl url("test.com", "/page", Protocol::HTTP);
	EXPECT_EQ(url.GetProtocol(), Protocol::HTTP);
	EXPECT_EQ(url.GetPort(), 80);
	EXPECT_EQ(url.GetURL(), "http://test.com/page");
}

TEST(HttpUrlParser, GetProtocolStringCorrectness)
{
	const HttpUrl httpUrl("http://a.com");
	const HttpUrl httpsUrl("https://a.com");

	EXPECT_EQ(httpUrl.GetProtocolString(), "http");
	EXPECT_EQ(httpsUrl.GetProtocolString(), "https");
}

TEST(HttpUrlParser, EmptyPortAfterColon)
{
	ASSERT_THROW(HttpUrl("http://domain:/path"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://domain:"), UrlParsingError);
}

TEST(HttpUrlParser, DocumentPreservesCaseAndSpecialChars)
{
	const HttpUrl url("http://a.com/Path/To/File.HTML?query=1#frag");
	EXPECT_EQ(url.GetDocument(), "/Path/To/File.HTML?query=1#frag");
	EXPECT_EQ(url.GetURL(), "http://a.com/Path/To/File.HTML?query=1#frag");
}

TEST(HttpUrlParser, SubdomainAndLongDomain)
{
	const HttpUrl url("http://sub.domain.example.test/path");
	EXPECT_EQ(url.GetDomain(), "sub.domain.example.test");
	EXPECT_EQ(url.GetDocument(), "/path");
}

TEST(HttpUrlParser, UrlWithOnlyProtocolAndDomain)
{
	const HttpUrl url("https://secure.test");
	EXPECT_EQ(url.GetProtocol(), Protocol::HTTPS);
	EXPECT_EQ(url.GetDomain(), "secure.test");
	EXPECT_EQ(url.GetPort(), 443);
	EXPECT_EQ(url.GetDocument(), "/");
	EXPECT_EQ(url.GetURL(), "https://secure.test/");
}

TEST(HttpUrlBoundary, PortBoundaryValues)
{
	ASSERT_THROW(HttpUrl("http://a.com:0"), UrlParsingError);
	ASSERT_THROW(HttpUrl("http://a.com:65536"), UrlParsingError);

	const HttpUrl minValid("http://a.com:1");
	EXPECT_EQ(minValid.GetPort(), 1);

	const HttpUrl maxValid("http://a.com:65535");
	EXPECT_EQ(maxValid.GetPort(), 65535);
}

TEST(HttpUrlBoundary, DocumentNormalizationEdgeCases)
{
	const HttpUrl emptyDoc("http://a.com", "", Protocol::HTTP);
	EXPECT_EQ(emptyDoc.GetDocument(), "/");

	const HttpUrl slashOnly("http://a.com", "/", Protocol::HTTP);
	EXPECT_EQ(slashOnly.GetDocument(), "/");

	const HttpUrl noSlash("http://a.com", "file", Protocol::HTTP);
	EXPECT_EQ(noSlash.GetDocument(), "/file");

	const HttpUrl withSlash("http://a.com", "/file", Protocol::HTTP);
	EXPECT_EQ(withSlash.GetDocument(), "/file");
}

TEST(HttpUrlComponents, ComponentConstructorWithDefaultPort)
{
	const HttpUrl url1("example.com", "/doc", Protocol::HTTP);
	EXPECT_EQ(url1.GetPort(), 80);

	const HttpUrl url2("example.com", "/doc", Protocol::HTTPS);
	EXPECT_EQ(url2.GetPort(), 443);
}

TEST(HttpUrlComponents, ComponentConstructorThrowsOnInvalid)
{
	ASSERT_THROW(HttpUrl("", "", Protocol::HTTP), std::invalid_argument);
	ASSERT_THROW(HttpUrl("domain", "doc", Protocol::HTTP, 0), std::invalid_argument);
	ASSERT_THROW(HttpUrl("domain", "doc", Protocol::HTTP, 65536), std::invalid_argument);
}