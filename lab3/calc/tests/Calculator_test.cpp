#include "../src/Calculator.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <optional>
#include <sstream>

TEST(CalculatorTest, Variable_Basic)
{
	Calculator calc;
	EXPECT_TRUE(calc.DeclareVariable("x"));
	const auto value = calc.GetValue("x");
	EXPECT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));

	EXPECT_FALSE(calc.DeclareVariable("x"));

	EXPECT_TRUE(calc.SetVariable("y", 42.5));
	EXPECT_DOUBLE_EQ(calc.GetValue("y").value(), 42.5);

	EXPECT_TRUE(calc.SetVariable("z", 10.0));
	const Function func("z");
	calc.DeclareFunction("f", func);
	EXPECT_FALSE(calc.SetVariable("f", 5.0));
}

TEST(CalculatorTest, Function_Basic)
{
	Calculator calc;
	calc.SetVariable("x", 3.0);
	calc.SetVariable("y", 4.0);
	Function func("x", "y", '+');
	calc.DeclareFunction("sum", func);
	EXPECT_DOUBLE_EQ(calc.GetValue("sum").value(), 7.0);

	calc.DeclareVariable("undef");
	Function unary("undef");
	calc.DeclareFunction("f", unary);
	auto val = calc.GetValue("f");
	EXPECT_TRUE(val.has_value());
	EXPECT_FALSE(std::isfinite(val.value()));

	Function div("x", "y", '/');
	calc.DeclareFunction("d", div);
	calc.SetVariable("y", 0.0);
	auto divVal = calc.GetValue("d");
	EXPECT_TRUE(divVal.has_value());
	EXPECT_FALSE(std::isfinite(divVal.value()));
}

TEST(CalculatorTest, Function_Chain)
{
	Calculator calc;
	calc.SetVariable("x", 1.0);
	calc.SetVariable("y", 2.0);
	calc.SetVariable("z", 3.0);
	const Function f1("x", "y", '+');
	const Function f2("sum", "z", '+');
	calc.DeclareFunction("sum", f1);
	calc.DeclareFunction("total", f2);
	EXPECT_DOUBLE_EQ(calc.GetValue("total").value(), 6.0);

	EXPECT_FALSE(calc.GetValue("nonexistent").has_value());
}

TEST(CalculatorTest, Identifier_Validation)
{
	EXPECT_TRUE(Calculator::IsValidIdentifier("x"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("var123"));
	EXPECT_TRUE(Calculator::IsValidIdentifier("_private"));
	EXPECT_FALSE(Calculator::IsValidIdentifier(""));
	EXPECT_FALSE(Calculator::IsValidIdentifier("123start"));
	EXPECT_FALSE(Calculator::IsValidIdentifier("my-var"));
}

TEST(CalculatorTest, PrintVariables)
{
	Calculator calc;
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	calc.SetVariable("x", 10.0);
	calc.SetVariable("y", 20.0);
	calc.PrintVariables();
	std::string output = out.str();
	EXPECT_NE(output.find("x:10.00"), std::string::npos);
	EXPECT_NE(output.find("y:20.00"), std::string::npos);

	out.str("");
	calc.DeclareVariable("z");
	calc.PrintVariables();
	EXPECT_NE(out.str().find("z:nan"), std::string::npos);

	out.str("");
	calc.SetVariable("z", 3.0);
	calc.SetVariable("a", 1.0);
	calc.SetVariable("m", 2.0);
	calc.PrintVariables();
	output = out.str();
	EXPECT_LT(output.find("a:"), output.find("m:"));
	EXPECT_LT(output.find("m:"), output.find("z:"));

	std::cout.rdbuf(original);
}

TEST(CalculatorTest, PrintFunctions)
{
	Calculator calc;
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	calc.SetVariable("x", 5.0);
	calc.SetVariable("y", 3.0);
	Function func("x", "y", '+');
	calc.DeclareFunction("sum", func);
	calc.PrintFunctions();
	EXPECT_NE(out.str().find("sum:8.00"), std::string::npos);

	out.str("");
	Function div("x", "y", '/');
	calc.DeclareFunction("d", div);
	calc.SetVariable("y", 0.0);
	calc.PrintFunctions();
	EXPECT_NE(out.str().find("d:nan"), std::string::npos);

	out.str("");
	Function f1("x"), f2("x"), f3("x");
	calc.DeclareFunction("z", f1);
	calc.DeclareFunction("a", f2);
	calc.DeclareFunction("m", f3);
	calc.PrintFunctions();
	std::string output = out.str();
	EXPECT_LT(output.find("a:"), output.find("m:"));
	EXPECT_LT(output.find("m:"), output.find("z:"));

	std::cout.rdbuf(original);
}

TEST(CalculatorTest, Precision_And_Values)
{
	Calculator calc;
	calc.SetVariable("x", 1.234567);
	EXPECT_DOUBLE_EQ(calc.GetValue("x").value(), 1.234567);

	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());
	calc.PrintVariables();
	EXPECT_NE(out.str().find("x:1.23"), std::string::npos);
	std::cout.rdbuf(original);

	calc.SetVariable("neg", -15.5);
	EXPECT_DOUBLE_EQ(calc.GetValue("neg").value(), -15.5);

	calc.SetVariable("zero", 0.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("zero").value(), 0.0);

	calc.SetVariable("large", 1e100);
	EXPECT_DOUBLE_EQ(calc.GetValue("large").value(), 1e100);

	calc.SetVariable("small", 1e-100);
	EXPECT_DOUBLE_EQ(calc.GetValue("small").value(), 1e-100);
}

TEST(CalculatorTest, NameConflicts)
{
	Calculator calc;
	calc.DeclareVariable("x");
	const Function func("x");
	EXPECT_FALSE(calc.DeclareFunction("x", func));

	calc.DeclareVariable("y");
	calc.DeclareFunction("f", func);
	EXPECT_FALSE(calc.DeclareVariable("f"));
}

TEST(CalculatorTest, AllOperations)
{
	Calculator calc;
	calc.SetVariable("a", 10.0);
	calc.SetVariable("b", 3.0);
	calc.DeclareFunction("add", Function("a", "b", '+'));
	calc.DeclareFunction("sub", Function("a", "b", '-'));
	calc.DeclareFunction("mul", Function("a", "b", '*'));
	calc.DeclareFunction("div", Function("a", "b", '/'));

	EXPECT_DOUBLE_EQ(calc.GetValue("add").value(), 13.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sub").value(), 7.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("mul").value(), 30.0);
	EXPECT_NEAR(calc.GetValue("div").value(), 3.333333, 0.001);
}