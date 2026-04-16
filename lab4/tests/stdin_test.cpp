#include <gtest/gtest.h>
#include "../src/ShapeParser/ShapeParser.h"
#include <sstream>
#include <memory>

constexpr double kEpsilon = 1e-3;

TEST(ShapeParserTest, ParsesAllSupportedShapeTypes)
{
	const std::string input = R"(
		rectangle 10 10 20 20 ff0000 00ff00
		circle 100 100 10 000000 ffffff
		triangle 0 0 30 0 0 40 aabbcc ddeeff
		line 0 0 100 100 112233
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 4);

	EXPECT_NEAR(shapes[0]->GetArea(), 100.0, kEpsilon);
	EXPECT_EQ(shapes[0]->GetOutlineColor(), 0xFF0000);

	EXPECT_NEAR(shapes[1]->GetArea(), 314.0, kEpsilon);

	EXPECT_NEAR(shapes[2]->GetArea(), 600.0, kEpsilon);

	EXPECT_NEAR(shapes[3]->GetArea(), 0.0, kEpsilon);
}

TEST(ShapeParserTest, SkipsInvalidAndUnknownTypes)
{
	const std::string input = R"(
		rectangle 0 0 10 10 ff0000 00ff00
		nottype 0 0 1 ff0000 00ff00
		circle 50 50 5 112233 445566
		triangle 0 0 1 1 2 2 000000 ffffff
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 2);
}

TEST(ShapeParserTest, IncorrectPosition)
{
	const std::string input = R"(
		rectangle 0 0 10 10 ff0000 00ff00
		rectangle -1 -1 1 ff0000 00ff00
		circle 50 50 5 112233 445566
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
		line 0 0 10 112233
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 0);
}

TEST(ShapeParserTest, StrictHexColorValidation)
{
	const std::string input = R"(
		circle 0 0 1 ff0000 00ff00
		circle 0 0 1 ff00zz 00ff00
		circle 0 0 1 ff00000 00ff00
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_EQ(shapes.size(), 1);
}

TEST(ShapeParserTest, EmptyInput)
{
	const std::string input = R"(
	)";

	std::istringstream stream(input);
	const auto shapes = ShapeParser::Parse(stream);

	ASSERT_TRUE(shapes.empty());
}