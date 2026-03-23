#include "../src/parse_url.h"
#include <gtest/gtest.h>

TEST(ParseUrlTest, BasicHttpWithoutPortAndDoc)
{
	UrlInfo info;
	EXPECT_TRUE(ParseUrl("http://example.com", info));

	EXPECT_EQ(info.protocol, HTTP);
	EXPECT_EQ(info.port, 80);
	EXPECT_EQ(info.host, "example.com");
	EXPECT_EQ(info.document, "");
}

TEST(ParseUrlTest, HttpWithPortAndDoc)
{
	UrlInfo info;
	EXPECT_TRUE(ParseUrl("http://example.com:8080/index.html", info));

	EXPECT_EQ(info.protocol, HTTP);
	EXPECT_EQ(info.port, 8080);
	EXPECT_EQ(info.host, "example.com");
	EXPECT_EQ(info.document, "index.html");
}

TEST(ParseUrlTest, InvalidProtocol)
{
	UrlInfo info;
	EXPECT_FALSE(ParseUrl("htp://example.com", info));
	EXPECT_FALSE(ParseUrl("http:// example.com", info));
}

TEST(ParseUrlTest, EmptyHost)
{
	UrlInfo info;
	EXPECT_FALSE(ParseUrl("http:///path", info));
}

TEST(ParseUrlTest, GetInDocumentPath)
{
	UrlInfo info;
	const std::string url = "http://www.example.com/document1.html?page=30&lang=ru";

	EXPECT_TRUE(ParseUrl(url, info));

	EXPECT_EQ(info.protocol, HTTP);
	EXPECT_EQ(info.port, 80);
	EXPECT_EQ(info.host, "www.example.com");
	EXPECT_EQ(info.document, "document1.html?page=30&lang=ru");
}

TEST(ParseUrlTest, HttpsDefaultPort)
{
	UrlInfo info;
	EXPECT_TRUE(ParseUrl("https://secured.site.com", info));

	EXPECT_EQ(info.protocol, HTTPS);
	EXPECT_EQ(info.port, 443);
}

TEST(ParseUrlTest, FtpWithPort)
{
	UrlInfo info;
	EXPECT_TRUE(ParseUrl("ftp://files.server.net:10/test/zip.zip", info));

	EXPECT_EQ(info.protocol, FTP);
	EXPECT_EQ(info.port, 10);
	EXPECT_EQ(info.host, "files.server.net");
	EXPECT_EQ(info.document, "test/zip.zip");
}

TEST(ParseUrlTest, PortOutOfRange)
{
	UrlInfo info;
	EXPECT_FALSE(ParseUrl("http://example.com:70000", info));
}

// TODO: тест на границы и +1 + дефолтный у ftp

TEST(ParseUrlTest, FtpDefault)
{
	UrlInfo info;
	EXPECT_TRUE(ParseUrl("ftp://files.server.net", info));

	EXPECT_EQ(info.protocol, FTP);
	EXPECT_EQ(info.port, 21);
}

TEST(ParseUrlTest, LimitValue)
{
	UrlInfo info;
	EXPECT_FALSE(ParseUrl("htp://example.com", info));
	EXPECT_FALSE(ParseUrl("http:// example.com", info));
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
