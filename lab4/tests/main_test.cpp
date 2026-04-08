#include "Rectangle.h"
#include <gtest/gtest.h>

TEST(ShapeBaseTest, InheritedColorsWorkCorrectly)
{
	const uint32_t stroke = 0xFF0000;
	const uint32_t fill = 0x00FF00;
	Rectangle rect(0.0, 0.0, 10.0, 5.0, stroke, fill);

	EXPECT_EQ(rect.GetStrokeColor(), stroke);
	EXPECT_EQ(rect.GetFillColor(), fill);
}