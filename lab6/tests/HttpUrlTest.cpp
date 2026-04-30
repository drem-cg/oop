#include "HttpUrl.h"
#include "UrlParsingError.h"
#include <gtest/gtest.h>

TEST(HttpUrlParser, ValidUrlsFromString)
{
	CHttpUrl url1("http://test.com");
	EXPECT_EQ(url1.GetProtocol(), Protocol::HTTP);
	EXPECT_EQ(url1.GetDomain(), "test.com");
	EXPECT_EQ(url1.GetPort(), 80);
	EXPECT_EQ(url1.GetDocument(), "/");
	EXPECT_EQ(url1.GetURL(), "http://test.com/");

	CHttpUrl url2("HTTPS://test.COM:443/path");
	EXPECT_EQ(url2.GetProtocol(), Protocol::HTTPS);
	EXPECT_EQ(url2.GetDomain(), "test.COM");
	EXPECT_EQ(url2.GetPort(), 443);
	EXPECT_EQ(url2.GetDocument(), "/path");
	EXPECT_EQ(url2.GetURL(), "https://test.COM/path");

	CHttpUrl url3("http://host.org:8080/index.html");
	EXPECT_EQ(url3.GetPort(), 8080);
	EXPECT_EQ(url3.GetURL(), "http://host.org:8080/index.html");
}

TEST(HttpUrlParser, InvalidUrlsFromString)
{
	ASSERT_THROW(CHttpUrl("ftp://test.com"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http//test.com"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http://:80/path"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http://domain:abc"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http://domain:99999"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http://domain:0/path"), UrlParsingError);
	ASSERT_THROW(CHttpUrl("http://test .com"), UrlParsingError);
}

TEST(HttpUrlComponents, ValidConstruction)
{
	CHttpUrl url1("test.com", "index.html", Protocol::HTTP);
	EXPECT_EQ(url1.GetDocument(), "/index.html");
	EXPECT_EQ(url1.GetPort(), 80);

	CHttpUrl url2("test.com", "", Protocol::HTTPS, 8443);
	EXPECT_EQ(url2.GetDocument(), "/");
	EXPECT_EQ(url2.GetPort(), 8443);
}

TEST(HttpUrlComponents, InvalidConstruction)
{
	ASSERT_THROW(CHttpUrl("", "/doc", Protocol::HTTP), std::invalid_argument);
	ASSERT_THROW(CHttpUrl("domain", "/doc", Protocol::HTTP, 0), std::invalid_argument);
	ASSERT_THROW(CHttpUrl("domain", "/doc", Protocol::HTTP, 65536), std::invalid_argument);
}

TEST(HttpUrlBoundary, PortLimitsAndNormalization)
{
	CHttpUrl minPort("http://a.com:1");
	EXPECT_EQ(minPort.GetPort(), 1);

	CHttpUrl maxPort("http://a.com:65535");
	EXPECT_EQ(maxPort.GetPort(), 65535);

	CHttpUrl emptyDoc("http://a.com");
	EXPECT_EQ(emptyDoc.GetDocument(), "/");

	CHttpUrl noSlashDoc("http://a.com", "page.html", Protocol::HTTP);
	EXPECT_EQ(noSlashDoc.GetDocument(), "/page.html");

	CHttpUrl alreadySlashDoc("http://a.com", "/doc", Protocol::HTTP);
	EXPECT_EQ(alreadySlashDoc.GetDocument(), "/doc");
}