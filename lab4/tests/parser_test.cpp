#include "ShapeParser.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(ShapeParserTest, ParsesValidShapes)
{
	const std::string input = R"(
        rectangle 0 0 10 5 ff0000 00ff00
        circle 1 1 2.5 000000 ffffff
        # this is a comment
        ellipse 0 0 4 2 abcdef 123456
    )";

	ShapeParser parser;
	std::istringstream stream(input);
	const auto shapes = parser.Parse(stream);

	ASSERT_EQ(shapes.size(), 3);
	EXPECT_NEAR(shapes[0]->GetArea(), 50.0, 1e-5);
	EXPECT_NEAR(shapes[1]->GetPerimeter(), 15.7079, 1e-4);
	EXPECT_EQ(shapes[2]->GetStrokeColor(), 0xABCDEF);
}

TEST(ShapeParserTest, HandlesInvalidLinesGracefully)
{
	const std::string input = R"(
        badtype 0 0 1 ff0000 00ff00
        rectangle 0 0 -1 5 ff0000 00ff00
        circle 0 0 2 000000
        ellipse 0 0 2 2 ff0000 00ff00
    )";

	ShapeParser parser;
	std::istringstream stream(input);
	const auto shapes = parser.Parse(stream);

	ASSERT_EQ(shapes.size(), 1);
	EXPECT_NEAR(shapes[0]->GetArea(), 3.14159 * 2.0 * 2.0, 1e-4);
}