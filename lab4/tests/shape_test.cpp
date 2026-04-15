#include "Circle/CCircle.h"
#include "LineSegment/CLineSegment.h"
#include "Rectangle/CRectangle.h"
#include "Triangle/CTriangle.h"
#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

constexpr double kPi = 3.14;
constexpr double kEpsilon = 1e-3;

TEST(CRectangleTest, ValidCreationAndCalculations)
{
	const CRectangle rect(CPoint{ 0.0, 0.0 }, CPoint{ 10.0, 5.0 }, 0xFF0000, 0x00FF00);

	EXPECT_NEAR(rect.GetArea(), 50.0, kEpsilon);
	EXPECT_NEAR(rect.GetPerimeter(), 30.0, kEpsilon);
	EXPECT_EQ(rect.GetOutlineColor(), 0xFF0000);
	EXPECT_EQ(rect.GetFillColor(), 0x00FF00);
	EXPECT_EQ(rect.ToString(), "Rectangle (leftTop=(0.00,0.00), width=10.00, height=5.00)");
}

TEST(CRectangleTest, InvalidDimensionsThrow)
{
	EXPECT_THROW(CRectangle(CPoint{ 0, 0 }, CPoint{ 0, 5 }, 0, 0), std::invalid_argument);
	EXPECT_THROW(CRectangle(CPoint{ 0, 0 }, CPoint{ 10, 0 }, 0, 0), std::invalid_argument);
}

TEST(CCircleTest, ValidCreationAndCalculations)
{
	const CCircle circle(CPoint{ 10.0, 20.0 }, 3.0, 0x000000, 0xFFFFFF);

	EXPECT_NEAR(circle.GetArea(), kPi * 9.0, kEpsilon);
	EXPECT_NEAR(circle.GetPerimeter(), 2.0 * kPi * 3.0, kEpsilon);
	EXPECT_EQ(circle.GetOutlineColor(), 0x000000);
	EXPECT_EQ(circle.GetFillColor(), 0xFFFFFF);
}

TEST(CCircleTest, InvalidRadiusThrows)
{
	EXPECT_THROW(CCircle(CPoint{ 0, 0 }, 0.0, 0, 0), std::invalid_argument);
	EXPECT_THROW(CCircle(CPoint{ 0, 0 }, -2.5, 0, 0), std::invalid_argument);
}

TEST(CTriangleTest, ValidCreationAndCalculations)
{
	const CTriangle tri(CPoint{ 0.0, 0.0 }, CPoint{ 3.0, 0.0 }, CPoint{ 0.0, 4.0 }, 0x111111, 0x222222);

	EXPECT_NEAR(tri.GetArea(), 6.0, kEpsilon);
	EXPECT_NEAR(tri.GetPerimeter(), 12.0, kEpsilon);
	EXPECT_EQ(tri.GetOutlineColor(), 0x111111);
	EXPECT_EQ(tri.GetFillColor(), 0x222222);
}

TEST(CTriangleTest, DegenerateThrows)
{
	EXPECT_THROW(CTriangle(CPoint{ 0, 0 }, CPoint{ 1, 1 }, CPoint{ 2, 2 }, 0, 0), std::invalid_argument);
}

TEST(CLineSegmentTest, ValidCreationAndCalculations)
{
	const CLineSegment line(CPoint{ 0.0, 0.0 }, CPoint{ 3.0, 4.0 }, 0xABCDEF);

	EXPECT_NEAR(line.GetArea(), 0.0, kEpsilon);
	EXPECT_NEAR(line.GetPerimeter(), 5.0, kEpsilon);
	EXPECT_EQ(line.GetOutlineColor(), 0xABCDEF);
}