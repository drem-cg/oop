#include <gtest/gtest.h>
#include "../src/ShapeParser/ShapeParser.h"
#include <sstream>
#include <memory>
#include <cmath>

constexpr double kEpsilon = 1e-9;

TEST(ShapeParserTest, ParsesAllShapeTypesCorrectly)
{
	const std::string input = R"(
		rectangle 10 20 5 5 ff0000 00ff00
		circle 0 0 2 000000 ffffff
		triangle 0 0 3 0 0 4 aabbcc ddeeff
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 3);

	EXPECT_EQ(shapes[0]->GetOutlineColor(), 0xFF0000);

	EXPECT_NEAR(shapes[0]->GetArea(), 25.0, kEpsilon);
	EXPECT_NEAR(shapes[1]->GetArea(), 3.14 * 4.0, kEpsilon);
	EXPECT_NEAR(shapes[2]->GetArea(), 6.0, kEpsilon);
	EXPECT_NEAR(shapes[3]->GetArea(), 3.14 * 20.0, kEpsilon);
}

TEST(ShapeParserTest, ValidAndInvalidInput)
{
	const std::string input = R"(
		rectangle 0 0 10 10 ff0000 00ff00
		badtype 0 0 1 ff0000 00ff00
		circle 0 0 1 ff0000 00ff00
		triangle 0 0 1 1 2 2 000000 ffffff
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 2);
}

TEST(ShapeParserTest, MissingParameters)
{
	const std::string input = R"(
		rectangle 0 0 10 ff0000 00ff00
		circle 0 0 ff0000 00ff00
		ellipse 0 0 10 2 ff0000
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 0);
}

TEST(ShapeParserTest, ParsesUntilEOF)
{
	const std::string input = "circle 0 0 1 aabbcc ddeeff";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 1);
	EXPECT_NEAR(shapes[0]->GetArea(), 3.14, kEpsilon);
}