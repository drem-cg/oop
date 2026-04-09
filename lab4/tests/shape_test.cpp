#include "Circle.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <cmath>
#include <gtest/gtest.h>

constexpr double kPi = 3.14;
constexpr double kEpsilon = 1e-3;

TEST(RectangleTest, ValidCreationAndCalculations)
{
	const Rectangle rect(0.0, 0.0, 4.0, 5.0, 0xFF0000, 0x00FF00);
	EXPECT_NEAR(rect.GetArea(), 20.0, kEpsilon);
	EXPECT_NEAR(rect.GetPerimeter(), 18.0, kEpsilon);
	EXPECT_EQ(rect.ToString(), "Rectangle (x=0.00, y=0.00, w=4.00, h=5.00)");
}

TEST(RectangleTest, InvalidDimensionsThrows)
{
	EXPECT_THROW(Rectangle(0, 0, -1.0, 5.0, 0, 0), std::invalid_argument);
	EXPECT_THROW(Rectangle(0, 0, 1.0, 0.0, 0, 0), std::invalid_argument);
}

TEST(CircleTest, ValidCreationAndCalculations)
{
	const Circle circle(10.0, 20.0, 3.0, 0x000000, 0xFFFFFF);
	EXPECT_NEAR(circle.GetArea(), kPi * 9.0, kEpsilon);
	EXPECT_NEAR(circle.GetPerimeter(), 2.0 * kPi * 3.0, kEpsilon);
}

TEST(CircleTest, InvalidRadiusThrows)
{
	EXPECT_THROW(Circle(0, 0, 0.0, 0, 0), std::invalid_argument);
	EXPECT_THROW(Circle(0, 0, -2.5, 0, 0), std::invalid_argument);
}

TEST(TriangleTest, ValidCreationAndCalculations)
{
	const Triangle tri(0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0x111111, 0x222222);
	EXPECT_NEAR(tri.GetArea(), 6.0, kEpsilon);
	EXPECT_NEAR(tri.GetPerimeter(), 12.0, kEpsilon);
}

TEST(TriangleTest, DegenerateThrows)
{
	EXPECT_THROW(Triangle(0, 0, 1, 1, 2, 2, 0, 0), std::invalid_argument);
}

TEST(EllipseTest, ValidCreationAndCalculations)
{
	const Ellipse ellipse(0.0, 0.0, 4.0, 2.0, 0xABCDEF, 0x123456);
	constexpr double expectedArea = kPi * 8.0;
	EXPECT_NEAR(ellipse.GetArea(), expectedArea, kEpsilon);

	constexpr double h = (4.0 - 2.0) * (4.0 - 2.0) / ((4.0 + 2.0) * (4.0 + 2.0));
	const double expectedPerimeter = kPi * (4.0 + 2.0) * (1.0 + 3.0 * h / (10.0 + std::sqrt(4.0 - 3.0 * h)));
	EXPECT_NEAR(ellipse.GetPerimeter(), expectedPerimeter, kEpsilon);
}

TEST(EllipseTest, InvalidRadiiThrows)
{
	EXPECT_THROW(Ellipse(0, 0, 0.0, 2.0, 0, 0), std::invalid_argument);
	EXPECT_THROW(Ellipse(0, 0, 5.0, -1.0, 0, 0), std::invalid_argument);
}