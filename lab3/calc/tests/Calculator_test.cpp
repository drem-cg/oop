#include "../src/Calculator.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <optional>
#include <sstream>

class CalculatorTest : public ::testing::Test
{
protected:
	Calculator calc;
	std::streambuf* originalCout;
	std::ostringstream testOutput;

	void SetUp() override
	{
		originalCout = std::cout.rdbuf(testOutput.rdbuf());
	}

	void TearDown() override
	{
		std::cout.rdbuf(originalCout);
		testOutput.str("");
		testOutput.clear();
	}

	std::string GetOutput() const
	{
		return testOutput.str();
	}
};

TEST_F(CalculatorTest, DeclareVariable_CreatesUndefinedVariable)
{
	EXPECT_TRUE(calc.DeclareVariable("x"));
	auto value = calc.GetValue("x");
	EXPECT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));
}

TEST_F(CalculatorTest, DeclareVariable_DuplicateReturnsFalse)
{
	calc.DeclareVariable("x");
	testOutput.str("");
	EXPECT_FALSE(calc.DeclareVariable("x"));
}

TEST_F(CalculatorTest, SetVariable_AssignsValue)
{
	EXPECT_TRUE(calc.SetVariable("x", 42.5));
	auto value = calc.GetValue("x");
	EXPECT_TRUE(value.has_value());
	EXPECT_TRUE(std::isfinite(value.value()));
	EXPECT_DOUBLE_EQ(value.value(), 42.5);
}

TEST_F(CalculatorTest, SetVariable_AutoDeclaresIfNotExists)
{
	EXPECT_TRUE(calc.SetVariable("y", 10.0));
	auto value = calc.GetValue("y");
	EXPECT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 10.0);
}

TEST_F(CalculatorTest, SetVariable_CannotOverwriteFunction)
{
	calc.DeclareVariable("x");
	Function func("x");
	calc.DeclareFunction("f", func);
	EXPECT_FALSE(calc.SetVariable("f", 5.0));
}

TEST_F(CalculatorTest, DeclareFunction_UnaryFunction)
{
	calc.DeclareVariable("x");
	Function func("x");
	EXPECT_TRUE(calc.DeclareFunction("f", func));
}

TEST_F(CalculatorTest, DeclareFunction_BinaryFunction)
{
	calc.DeclareVariable("x");
	calc.DeclareVariable("y");
	Function func("x", "y", '+');
	EXPECT_TRUE(calc.DeclareFunction("sum", func));
}

TEST_F(CalculatorTest, DeclareFunction_DuplicateReturnsFalse)
{
	calc.DeclareVariable("x");
	Function func("x");
	calc.DeclareFunction("f", func);
	testOutput.str("");
	EXPECT_FALSE(calc.DeclareFunction("f", func));
}

TEST_F(CalculatorTest, FunctionValue_DependsOnVariables)
{
	calc.SetVariable("x", 3.0);
	calc.SetVariable("y", 4.0);
	Function func("x", "y", '+');
	calc.DeclareFunction("sum", func);

	auto value = calc.GetValue("sum");
	ASSERT_TRUE(value.has_value()) << "Function value should have value";
	EXPECT_DOUBLE_EQ(value.value(), 7.0);
}

TEST_F(CalculatorTest, FunctionValue_UndefinedOperand_ReturnsNan)
{
	calc.SetVariable("x", 5.0);
	calc.DeclareVariable("y");
	Function func("x", "y", '+');
	calc.DeclareFunction("sum", func);

	auto value = calc.GetValue("sum");
	EXPECT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));
}

TEST_F(CalculatorTest, FunctionValue_DivisionByZero_ReturnsNan)
{
	calc.DeclareVariable("x");
	calc.DeclareVariable("y");
	Function func("x", "y", '/');
	calc.DeclareFunction("div", func);

	calc.SetVariable("x", 10.0);
	calc.SetVariable("y", 0.0);
	auto value = calc.GetValue("div");
	EXPECT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));
}

TEST_F(CalculatorTest, FunctionValue_DependsOnAnotherFunction)
{
	calc.DeclareVariable("x");
	calc.DeclareVariable("y");
	calc.DeclareVariable("z");

	Function func1("x", "y", '+');
	Function func2("sum", "z", '+');

	calc.DeclareFunction("sum", func1);
	calc.DeclareFunction("total", func2);

	calc.SetVariable("x", 1.0);
	calc.SetVariable("y", 2.0);
	calc.SetVariable("z", 3.0);

	auto value = calc.GetValue("total");
	ASSERT_TRUE(value.has_value()) << "Total function should have value";
	EXPECT_DOUBLE_EQ(value.value(), 6.0);
}

TEST_F(CalculatorTest, GetValue_NonExistentName_ReturnsNullopt)
{
	auto value = calc.GetValue("nonexistent");
	EXPECT_FALSE(value.has_value());
}

TEST_F(CalculatorTest, IsValidIdentifier_ValidNames)
{
	EXPECT_TRUE(Calculator::IsValidIdentifier("x"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("var123"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("_private"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("my_var_name"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("A1B2C3"));
}

TEST_F(CalculatorTest, IsValidIdentifier_InvalidNames)
{
	EXPECT_FALSE(Calculator::IsValidIdentifier(""));
	EXPECT_FALSE(Calculator::IsValidIdentifier("123start"));
	EXPECT_FALSE(Calculator::IsValidIdentifier("my-var"));
	EXPECT_FALSE(Calculator::IsValidIdentifier("my.var"));
	EXPECT_FALSE(Calculator::IsValidIdentifier("my var"));
}

TEST_F(CalculatorTest, PrintVariables_PrintsAllVariables)
{
	calc.SetVariable("x", 10.0);
	calc.SetVariable("y", 20.0);
	testOutput.str("");
	calc.PrintVariables();

	std::string output = GetOutput();
	EXPECT_NE(output.find("x:10.00"), std::string::npos) << "Output: " << output;
	EXPECT_NE(output.find("y:20.00"), std::string::npos) << "Output: " << output;
}

TEST_F(CalculatorTest, PrintVariables_PrintsNanForUndefined)
{
	calc.DeclareVariable("x");
	testOutput.str("");
	calc.PrintVariables();

	std::string output = GetOutput();
	EXPECT_NE(output.find("x:nan"), std::string::npos) << "Output: " << output;
}

TEST_F(CalculatorTest, PrintVariables_SortedAlphabetically)
{
	calc.SetVariable("z", 3.0);
	calc.SetVariable("a", 1.0);
	calc.SetVariable("m", 2.0);
	testOutput.str("");
	calc.PrintVariables();

	std::string output = GetOutput();
	size_t posA = output.find("a:");
	size_t posM = output.find("m:");
	size_t posZ = output.find("z:");

	EXPECT_LT(posA, posM);
	EXPECT_LT(posM, posZ);
}

TEST_F(CalculatorTest, PrintFunctions_PrintsAllFunctions)
{
	calc.DeclareVariable("x");
	calc.DeclareVariable("y");
	Function func("x", "y", '+');
	calc.DeclareFunction("sum", func);

	calc.SetVariable("x", 5.0);
	calc.SetVariable("y", 3.0);
	testOutput.str("");
	calc.PrintFunctions();

	std::string output = GetOutput();
	EXPECT_NE(output.find("sum:8.00"), std::string::npos) << "Output: " << output;
}

TEST_F(CalculatorTest, PrintFunctions_PrintsNanForUndefined)
{
	calc.DeclareVariable("x");
	calc.DeclareVariable("y");
	Function func("x", "y", '/');
	calc.DeclareFunction("div", func);

	calc.SetVariable("x", 10.0);
	calc.SetVariable("y", 0.0);
	testOutput.str("");
	calc.PrintFunctions();

	std::string output = GetOutput();
	EXPECT_NE(output.find("div:nan"), std::string::npos) << "Output: " << output;
}

TEST_F(CalculatorTest, PrintFunctions_SortedAlphabetically)
{
	calc.DeclareVariable("x");
	Function func1("x");
	Function func2("x");
	Function func3("x");

	calc.DeclareFunction("z", func1);
	calc.DeclareFunction("a", func2);
	calc.DeclareFunction("m", func3);

	calc.SetVariable("x", 1.0);
	testOutput.str("");
	calc.PrintFunctions();

	std::string output = GetOutput();
	size_t posA = output.find("a:");
	size_t posM = output.find("m:");
	size_t posZ = output.find("z:");

	EXPECT_LT(posA, posM);
	EXPECT_LT(posM, posZ);
}

TEST_F(CalculatorTest, GetValue_Precision_TwoDecimalPlaces)
{
	calc.SetVariable("x", 1.234567);
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 1.234567);

	testOutput.str("");
	calc.PrintVariables();
	std::string output = GetOutput();
	EXPECT_NE(output.find("x:1.23"), std::string::npos);
}

TEST_F(CalculatorTest, GetValue_NegativeValues)
{
	calc.SetVariable("x", -15.5);
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), -15.5);
}

TEST_F(CalculatorTest, GetValue_ZeroValue)
{
	calc.SetVariable("x", 0.0);
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 0.0);
}

TEST_F(CalculatorTest, CannotDeclareFunctionWithExistingVariableName)
{
	calc.DeclareVariable("x");
	Function func("x");
	EXPECT_FALSE(calc.DeclareFunction("x", func));
}

TEST_F(CalculatorTest, CannotDeclareVariableWithExistingFunctionName)
{
	calc.DeclareVariable("x");
	Function func("x");
	calc.DeclareFunction("f", func);
	EXPECT_FALSE(calc.DeclareVariable("f"));
}

// ==================== Edge Cases ====================

TEST_F(CalculatorTest, LargeValue)
{
	calc.SetVariable("x", 1e100);
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 1e100);
}

TEST_F(CalculatorTest, VerySmallValue)
{
	calc.SetVariable("x", 1e-100);
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 1e-100);
}

TEST_F(CalculatorTest, AllOperations)
{
	calc.DeclareVariable("a");
	calc.DeclareVariable("b");

	calc.SetVariable("a", 10.0);
	calc.SetVariable("b", 3.0);

	Function add("a", "b", '+');
	Function sub("a", "b", '-');
	Function mul("a", "b", '*');
	Function div("a", "b", '/');

	calc.DeclareFunction("add", add);
	calc.DeclareFunction("sub", sub);
	calc.DeclareFunction("mul", mul);
	calc.DeclareFunction("div", div);

	auto addVal = calc.GetValue("add");
	auto subVal = calc.GetValue("sub");
	auto mulVal = calc.GetValue("mul");
	auto divVal = calc.GetValue("div");

	ASSERT_TRUE(addVal.has_value());
	ASSERT_TRUE(subVal.has_value());
	ASSERT_TRUE(mulVal.has_value());
	ASSERT_TRUE(divVal.has_value());

	EXPECT_DOUBLE_EQ(addVal.value(), 13.0);
	EXPECT_DOUBLE_EQ(subVal.value(), 7.0);
	EXPECT_DOUBLE_EQ(mulVal.value(), 30.0);
	EXPECT_DOUBLE_EQ(divVal.value(), 3.3333333333333335);
}