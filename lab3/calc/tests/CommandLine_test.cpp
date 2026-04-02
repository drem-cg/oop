#include "../src/Calculator.h"
#include "../src/Parser.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>

TEST(CommandLineTest, Var_Basic)
{
	Calculator calc;
	const auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("var x");
	EXPECT_TRUE(calc.GetValue("x").has_value());
	EXPECT_FALSE(std::isfinite(calc.GetValue("x").value()));

	out.str("");
	parser->Parse("var ");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("var 123invalid");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("var x");
	EXPECT_EQ(out.str(), "Name already exists\n");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, Let_Basic)
{
	Calculator calc;
	auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("let x = 42.5");
	EXPECT_DOUBLE_EQ(calc.GetValue("x").value(), 42.5);

	parser->Parse("let y = x");
	EXPECT_DOUBLE_EQ(calc.GetValue("y").value(), 42.5);

	parser->Parse("let z = nonexistent");
	EXPECT_EQ(out.str(), "Name does not exist\n");

	out.str("");
	parser->Parse("let x 10");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("let = 10");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("let x =");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, Fn_Basic)
{
	Calculator calc;
	auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("var x");
	parser->Parse("var y");
	parser->Parse("fn f = x");
	parser->Parse("fn sum = x + y");
	parser->Parse("let x = 5");
	parser->Parse("let y = 3");

	EXPECT_DOUBLE_EQ(calc.GetValue("f").value(), 5.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sum").value(), 8.0);

	parser->Parse("var a");
	parser->Parse("var b");
	parser->Parse("fn div = a / b");
	parser->Parse("let a = 10");
	parser->Parse("let b = 0");
	auto divVal = calc.GetValue("div");
	EXPECT_TRUE(divVal.has_value());
	EXPECT_FALSE(std::isfinite(divVal.value()));

	out.str("");
	parser->Parse("fn f x");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("var z");
	parser->Parse("fn f = z");
	EXPECT_EQ(out.str(), "Name already exists\n");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, Print_Basic)
{
	Calculator calc;
	const auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("let x = 42");
	parser->Parse("print x");
	EXPECT_EQ(out.str(), "42.00\n");

	out.str("");
	parser->Parse("var y");
	parser->Parse("print y");
	EXPECT_EQ(out.str(), "nan\n");

	out.str("");
	parser->Parse("print nonexistent");
	EXPECT_EQ(out.str(), "nan\n");

	out.str("");
	parser->Parse("print ");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("let z = 1.234567");
	parser->Parse("print z");
	EXPECT_EQ(out.str(), "1.23\n");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, PrintVars_Printfns)
{
	Calculator calc;
	auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("let x = 10");
	parser->Parse("let y = 20");
	parser->Parse("printvars");
	std::string output = out.str();
	EXPECT_NE(output.find("x:10.00"), std::string::npos);
	EXPECT_NE(output.find("y:20.00"), std::string::npos);

	out.str("");
	parser->Parse("var z");
	parser->Parse("printvars");
	EXPECT_NE(out.str().find("z:nan"), std::string::npos);

	out.str("");
	parser->Parse("var a");
	parser->Parse("var b");
	parser->Parse("fn f = a + b");
	parser->Parse("let a = 5");
	parser->Parse("let b = 3");
	parser->Parse("printfns");
	EXPECT_NE(out.str().find("f:8.00"), std::string::npos);

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, UnknownAndEdgeCases)
{
	Calculator calc;
	auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("unknowncommand");
	EXPECT_EQ(out.str(), "Unknown command\n");

	out.str("");
	parser->Parse("let    x    =    42");
	EXPECT_DOUBLE_EQ(calc.GetValue("x").value(), 42.0);

	parser->Parse("var \t y \t ");
	EXPECT_TRUE(calc.GetValue("y").has_value());

	out.str("");
	parser->Parse("var 123start");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("var my-var");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	out.str("");
	parser->Parse("var my.var");
	EXPECT_EQ(out.str(), "Invalid usage\n");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, Scenario_FnVsLet)
{
	Calculator calc;
	auto parser = std::make_unique<Parser>(calc);
	std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("let v = 42");
	parser->Parse("let variable = v");
	parser->Parse("fn function = v");
	parser->Parse("let v = 43");
	parser->Parse("print variable");
	parser->Parse("print function");

	std::istringstream iss(out.str());
	std::string line;
	std::getline(iss, line);
	EXPECT_EQ(line, "42.00");
	std::getline(iss, line);
	EXPECT_EQ(line, "43.00");

	std::cout.rdbuf(original);
}

TEST(CommandLineTest, Scenario_Fibonacci)
{
	Calculator calc;
	const auto parser = std::make_unique<Parser>(calc);
	const std::ostringstream out;
	std::streambuf* original = std::cout.rdbuf(out.rdbuf());

	parser->Parse("let v0 = 0");
	parser->Parse("let v1 = 1");
	parser->Parse("fn fib0 = v0");
	parser->Parse("fn fib1 = v1");
	parser->Parse("fn fib2 = fib1 + fib0");
	parser->Parse("fn fib3 = fib2 + fib1");
	parser->Parse("fn fib4 = fib3 + fib2");
	parser->Parse("printfns");

	std::string output = out.str();
	EXPECT_NE(output.find("fib0:0.00"), std::string::npos);
	EXPECT_NE(output.find("fib4:3.00"), std::string::npos);

	std::cout.rdbuf(original);
}