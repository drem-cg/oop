#include "../src/html_decode.h"
#include <gtest/gtest.h>

TEST(HtmlDecodeTest, SingleEntities)
{
	EXPECT_EQ(HtmlDecode("&quot;"), "\"");
	EXPECT_EQ(HtmlDecode("&apos;"), "'");
	EXPECT_EQ(HtmlDecode("&amp;"), "&");
	EXPECT_EQ(HtmlDecode("&lt;"), "<");
	EXPECT_EQ(HtmlDecode("&gt;"), ">");
}

TEST(HtmlDecodeTest, MultipleEntities)
{
	EXPECT_EQ(HtmlDecode("Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s"),
		"Cat <says> \"Meow\". M&M's");
}

TEST(HtmlDecodeTest, NoEntities)
{
	EXPECT_EQ(HtmlDecode("Hello, world!"), "Hello, world!");
	EXPECT_EQ(HtmlDecode("This is a test"), "This is a test");
	EXPECT_EQ(HtmlDecode(""), "");
}

TEST(HtmlDecodeTest, PartialEntities)
{
	EXPECT_EQ(HtmlDecode("&qu"), "&qu");
	EXPECT_EQ(HtmlDecode("&ap"), "&ap");
	EXPECT_EQ(HtmlDecode("&am"), "&am");
	EXPECT_EQ(HtmlDecode("&l"), "&l");
	EXPECT_EQ(HtmlDecode("&g"), "&g");
	EXPECT_EQ(HtmlDecode("&xyz;"), "&xyz;");
}

TEST(HtmlDecodeTest, NestedEntities)
{
	EXPECT_EQ(HtmlDecode("&lt;"), "<");
	EXPECT_EQ(HtmlDecode("&gt;"), ">");
	EXPECT_EQ(HtmlDecode("&amp;"), "&");
	EXPECT_EQ(HtmlDecode("&quot;"), "\"");
	EXPECT_EQ(HtmlDecode("&apos;"), "'");
	EXPECT_EQ(HtmlDecode("&lt;div&gt;"), "<div>");
}

TEST(HtmlDecodeTest, ComplexCombinations)
{
	EXPECT_EQ(HtmlDecode("&lt;"), "<");
	EXPECT_EQ(HtmlDecode("&amp;"), "&");
	EXPECT_EQ(HtmlDecode("a&amp;b&amp;c"), "a&b&c");
	EXPECT_EQ(HtmlDecode("&lt;div class=&quot;test&quot;&gt;"), "<div class=\"test\">");
}
// TODO: тест на заглавную + повторение типа &&

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
