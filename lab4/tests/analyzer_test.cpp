#include "Circle.h"
#include "Rectangle.h"
#include "ShapeAnalyzer.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

TEST(ShapeAnalyzerTest, FindsMaxArea)
{
	const std::vector<std::shared_ptr<IShape>> shapes = {
		std::make_shared<Rectangle>(0, 0, 2, 2, 0, 0), // 4
		std::make_shared<Circle>(0, 0, 1, 0, 0), // ~3.14
		std::make_shared<Rectangle>(0, 0, 3, 3, 0, 0) // 9
	};

	ShapeAnalyzer analyzer;
	const auto result = analyzer.FindMaxArea(shapes);
	ASSERT_NE(result, nullptr);
	EXPECT_NEAR(result->GetArea(), 9.0, 1e-5);
}

TEST(ShapeAnalyzerTest, FindsMinPerimeter)
{
	const std::vector<std::shared_ptr<IShape>> shapes = {
		std::make_shared<Rectangle>(0, 0, 10, 10, 0, 0), // 40
		std::make_shared<Circle>(0, 0, 1, 0, 0), // ~6.28
		std::make_shared<Rectangle>(0, 0, 2, 2, 0, 0) // 8
	};

	ShapeAnalyzer analyzer;
	const auto result = analyzer.FindMinPerimeter(shapes);
	ASSERT_NE(result, nullptr);
	EXPECT_NEAR(result->GetPerimeter(), 6.28318, 1e-4);
}

TEST(ShapeAnalyzerTest, HandlesEmptyVector)
{
	ShapeAnalyzer analyzer;
	const std::vector<std::shared_ptr<IShape>> empty;
	EXPECT_EQ(analyzer.FindMaxArea(empty), nullptr);
	EXPECT_EQ(analyzer.FindMinPerimeter(empty), nullptr);
}