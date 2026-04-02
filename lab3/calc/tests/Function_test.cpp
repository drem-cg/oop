#include "../src/Function.h"
#include <gtest/gtest.h>
#include <string>

TEST(FunctionTest, Unary_Basic)
{
	const Function func("x");
	EXPECT_FALSE(func.IsBinary());
	EXPECT_EQ(func.GetOperand1(), "x");
	EXPECT_EQ(func.GetOperand2(), "");
	EXPECT_EQ(func.GetOperation(), '\0');
}

TEST(FunctionTest, Binary_Basic)
{
	const Function func("a", "b", '+');
	EXPECT_TRUE(func.IsBinary());
	EXPECT_EQ(func.GetOperand1(), "a");
	EXPECT_EQ(func.GetOperand2(), "b");
	EXPECT_EQ(func.GetOperation(), '+');
}

TEST(FunctionTest, AllOperations)
{
	EXPECT_EQ(Function("x", "y", '+').GetOperation(), '+');
	EXPECT_EQ(Function("x", "y", '-').GetOperation(), '-');
	EXPECT_EQ(Function("x", "y", '*').GetOperation(), '*');
	EXPECT_EQ(Function("x", "y", '/').GetOperation(), '/');
}

TEST(FunctionTest, OperandNames_Boundary)
{
	EXPECT_EQ(Function("a", "b", '+').GetOperand1(), "a");

	const std::string longName(100, 'x');
	EXPECT_EQ(Function(longName, "y", '*').GetOperand1(), longName);

	EXPECT_EQ(Function("_var1", "var_2", '-').GetOperand1(), "_var1");
	EXPECT_EQ(Function("_var1", "var_2", '-').GetOperand2(), "var_2");
}

TEST(FunctionTest, Immutable)
{
	const Function func("x", "y", '/');
	const Function& constFunc = func;
	EXPECT_EQ(constFunc.GetOperand1(), "x");
	EXPECT_EQ(constFunc.GetOperand2(), "y");
	EXPECT_EQ(constFunc.GetOperation(), '/');
	EXPECT_TRUE(constFunc.IsBinary());
}