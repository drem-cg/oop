#include "../src/Variable.h"
#include <cmath>
#include <gtest/gtest.h>

TEST(VariableTest, DefaultIsUndefined)
{
	const Variable var;
	EXPECT_FALSE(var.IsDefined());
	EXPECT_TRUE(std::isnan(var.GetValue()));
}

TEST(VariableTest, SetValue)
{
	Variable var;
	var.SetValue(1.0);
	EXPECT_TRUE(var.IsDefined());
	EXPECT_DOUBLE_EQ(var.GetValue(), 1.0);

	var.SetValue(20.0);
	EXPECT_DOUBLE_EQ(var.GetValue(), 20.0);
}

TEST(VariableTest, EdgeValues)
{
	Variable var;
	var.SetValue(-15.5);
	EXPECT_DOUBLE_EQ(var.GetValue(), -15.5);

	var.SetValue(0.0);
	EXPECT_TRUE(var.IsDefined());
	EXPECT_DOUBLE_EQ(var.GetValue(), 0.0);
}