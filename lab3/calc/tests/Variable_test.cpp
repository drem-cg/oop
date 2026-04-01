#include "../src/Variable.h"
#include <cmath>
#include <gtest/gtest.h>

class VariableTest : public ::testing::Test
{
protected:
	Variable var;
};

TEST_F(VariableTest, NewlyCreatedVariableIsUndefined)
{
	EXPECT_FALSE(var.IsDefined());
	EXPECT_TRUE(std::isnan(var.GetValue()));
}

TEST_F(VariableTest, SetValueMakesVariableDefined)
{
	var.SetValue(1.0);
	EXPECT_TRUE(var.IsDefined());
	EXPECT_DOUBLE_EQ(var.GetValue(), 1.0);
}

TEST_F(VariableTest, CanSetValueMultipleTimes)
{
	var.SetValue(10.0);
	EXPECT_DOUBLE_EQ(var.GetValue(), 10.0);

	var.SetValue(20.0);
	EXPECT_DOUBLE_EQ(var.GetValue(), 20.0);
}

TEST_F(VariableTest, CanSetNegativeValue)
{
	var.SetValue(-15.5);
	EXPECT_TRUE(var.IsDefined());
	EXPECT_DOUBLE_EQ(var.GetValue(), -15.5);
}

TEST_F(VariableTest, CanSetZeroValue)
{
	var.SetValue(0.0);
	EXPECT_TRUE(var.IsDefined());
	EXPECT_DOUBLE_EQ(var.GetValue(), 0.0);
}