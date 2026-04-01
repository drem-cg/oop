#include "../src/Function.h"
#include <gtest/gtest.h>
#include <string>

class FunctionTest : public ::testing::Test
{
};

TEST_F(FunctionTest, UnaryFunction_Basic)
{
	const Function func("x");
	EXPECT_FALSE(func.IsBinary());
	EXPECT_EQ(func.GetOperand1(), "x");
	EXPECT_EQ(func.GetOperand2(), "");
	EXPECT_EQ(func.GetOperation(), '\0');
}

TEST_F(FunctionTest, BinaryFunction_Basic)
{
	const Function func("a", "b", '+');
	EXPECT_TRUE(func.IsBinary());
	EXPECT_EQ(func.GetOperand1(), "a");
	EXPECT_EQ(func.GetOperand2(), "b");
	EXPECT_EQ(func.GetOperation(), '+');
}

TEST_F(FunctionTest, AllOperations)
{
	EXPECT_EQ(Function("x", "y", '+').GetOperation(), '+');
	EXPECT_EQ(Function("x", "y", '-').GetOperation(), '-');
	EXPECT_EQ(Function("x", "y", '*').GetOperation(), '*');
	EXPECT_EQ(Function("x", "y", '/').GetOperation(), '/');
}

TEST_F(FunctionTest, OperandNames_BoundaryCases)
{
	const Function shortName("a", "b", '+');
	EXPECT_EQ(shortName.GetOperand1(), "a");

	const std::string longName(100, 'x');
	const Function longOperand(longName, "y", '*');
	EXPECT_EQ(longOperand.GetOperand1(), longName);

	const Function withUnderscore("_var1", "var_2", '-');
	EXPECT_EQ(withUnderscore.GetOperand1(), "_var1");
	EXPECT_EQ(withUnderscore.GetOperand2(), "var_2");
}

TEST_F(FunctionTest, ImmutableAfterCreation)
{
	const Function func("x", "y", '/');
	const Function& constFunc = func;
	EXPECT_EQ(constFunc.GetOperand1(), "x");
	EXPECT_EQ(constFunc.GetOperand2(), "y");
	EXPECT_EQ(constFunc.GetOperation(), '/');
	EXPECT_TRUE(constFunc.IsBinary());
}