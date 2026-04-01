#include "../src/Calculator.h"
#include "../src/Parser.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

class CommandLineTest : public ::testing::Test
{
protected:
	Calculator calc;
	std::unique_ptr<Parser> parser;
	std::streambuf* originalCout;
	std::ostringstream testOutput;

	void SetUp() override
	{
		parser = std::make_unique<Parser>(calc);
		originalCout = std::cout.rdbuf(testOutput.rdbuf());
	}

	void TearDown() override
	{
		std::cout.rdbuf(originalCout);
		testOutput.str("");
		testOutput.clear();
	}

	std::string GetOutput() const { return testOutput.str(); }
	void ExecuteCommand(const std::string& command) { parser->Parse(command); }
	void ExecuteCommands(const std::vector<std::string>& commands)
	{
		for (const auto& cmd : commands)
			parser->Parse(cmd);
	}
	void ClearOutput()
	{
		testOutput.str("");
		testOutput.clear();
	}
};

TEST_F(CommandLineTest, Var_SimpleDeclaration)
{
	ExecuteCommand("var x");
	auto value = calc.GetValue("x");
	EXPECT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));
}

TEST_F(CommandLineTest, Var_EmptyName_InvalidUsage)
{
	ExecuteCommand("var ");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(CommandLineTest, Var_InvalidIdentifier_InvalidUsage)
{
	ExecuteCommand("var 123invalid");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(CommandLineTest, Var_DuplicateName_AlreadyExists)
{
	ExecuteCommand("var x");
	ClearOutput();
	ExecuteCommand("var x");
	EXPECT_EQ(GetOutput(), "Name already exists\n");
}

TEST_F(CommandLineTest, Let_AssignNumber)
{
	ExecuteCommand("let x = 42.5");
	auto value = calc.GetValue("x");
	ASSERT_TRUE(value.has_value());
	EXPECT_DOUBLE_EQ(value.value(), 42.5);
}

TEST_F(CommandLineTest, Let_AssignFromVariable)
{
	ExecuteCommands({ "let x = 10", "let y = x" });
	EXPECT_DOUBLE_EQ(calc.GetValue("y").value(), 10.0);
}

TEST_F(CommandLineTest, Let_SourceDoesNotExist)
{
	ExecuteCommand("let y = nonexistent");
	EXPECT_EQ(GetOutput(), "Name does not exist\n");
}

TEST_F(CommandLineTest, Let_NoEqualsSign_InvalidUsage)
{
	ExecuteCommand("let x 10");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(CommandLineTest, Fn_UnaryFunction)
{
	ExecuteCommands({ "var x", "fn f = x", "let x = 5" });
	EXPECT_DOUBLE_EQ(calc.GetValue("f").value(), 5.0);
}

TEST_F(CommandLineTest, Fn_BinaryFunction_Add)
{
	ExecuteCommands({ "var x", "var y", "fn sum = x + y", "let x = 3", "let y = 4" });
	EXPECT_DOUBLE_EQ(calc.GetValue("sum").value(), 7.0);
}

TEST_F(CommandLineTest, Fn_DivisionByZero_ReturnsNan)
{
	ExecuteCommands({ "var x", "var y", "fn div = x / y", "let x = 10", "let y = 0" });
	auto value = calc.GetValue("div");
	ASSERT_TRUE(value.has_value());
	EXPECT_FALSE(std::isfinite(value.value()));
}

TEST_F(CommandLineTest, Fn_NoEqualsSign_InvalidUsage)
{
	ExecuteCommand("fn f x");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(CommandLineTest, Print_PrintsValue)
{
	ExecuteCommand("let x = 42");
	ExecuteCommand("print x");
	EXPECT_EQ(GetOutput(), "42.00\n");
}

TEST_F(CommandLineTest, Print_PrintsNanForUndefined)
{
	ExecuteCommand("var x");
	ExecuteCommand("print x");
	EXPECT_EQ(GetOutput(), "nan\n");
}

TEST_F(CommandLineTest, Print_PrintsNanForNonExistent)
{
	ExecuteCommand("print nonexistent");
	EXPECT_EQ(GetOutput(), "nan\n");
}

TEST_F(CommandLineTest, Print_Precision_TwoDecimalPlaces)
{
	ExecuteCommand("let x = 1.234567");
	ExecuteCommand("print x");
	EXPECT_EQ(GetOutput(), "1.23\n");
}

TEST_F(CommandLineTest, PrintVars_PrintsAllVariables)
{
	ExecuteCommands({ "let x = 10", "let y = 20" });
	ClearOutput();
	ExecuteCommand("printvars");
	std::string output = GetOutput();
	EXPECT_NE(output.find("x:10.00"), std::string::npos);
	EXPECT_NE(output.find("y:20.00"), std::string::npos);
}

TEST_F(CommandLineTest, PrintVars_PrintsNanForUndefined)
{
	ExecuteCommand("var x");
	ClearOutput();
	ExecuteCommand("printvars");
	EXPECT_NE(GetOutput().find("x:nan"), std::string::npos);
}

TEST_F(CommandLineTest, PrintFns_PrintsAllFunctions)
{
	ExecuteCommands({ "var x", "var y", "fn sum = x + y", "let x = 5", "let y = 3" });
	ClearOutput();
	ExecuteCommand("printfns");
	EXPECT_NE(GetOutput().find("sum:8.00"), std::string::npos);
}

TEST_F(CommandLineTest, UnknownCommand_ReportsError)
{
	ExecuteCommand("unknowncommand");
	EXPECT_EQ(GetOutput(), "Unknown command\n");
}

TEST_F(CommandLineTest, Scenario_BasicVariableOperations)
{
	ExecuteCommands({ "var x", "print x", "let x = 42", "print x", "let x = 1.234", "print x" });
	std::istringstream iss(GetOutput());
	std::string line;
	std::getline(iss, line);
	EXPECT_EQ(line, "nan");
	std::getline(iss, line);
	EXPECT_EQ(line, "42.00");
	std::getline(iss, line);
	EXPECT_EQ(line, "1.23");
}

TEST_F(CommandLineTest, Scenario_FunctionDependencies)
{
	ExecuteCommands({ "var x", "var y", "fn XPlusY = x + y", "print XPlusY",
		"let x = 3", "let y = 4", "print XPlusY", "let x = 10", "print XPlusY" });
	std::istringstream iss(GetOutput());
	std::string line;
	std::getline(iss, line);
	EXPECT_EQ(line, "nan");
	std::getline(iss, line);
	EXPECT_EQ(line, "7.00");
	std::getline(iss, line);
	EXPECT_EQ(line, "14.00");
}

TEST_F(CommandLineTest, Scenario_FnVsLet_Difference)
{
	ExecuteCommands({ "let v = 42", "let variable = v", "fn function = v", "let v = 43",
		"print variable", "print function" });
	std::istringstream iss(GetOutput());
	std::string line;
	std::getline(iss, line);
	EXPECT_EQ(line, "42.00");
	std::getline(iss, line);
	EXPECT_EQ(line, "43.00");
}

TEST_F(CommandLineTest, Scenario_FibonacciSequence)
{
	ExecuteCommands({ "let v0 = 0", "let v1 = 1", "fn fib0 = v0", "fn fib1 = v1",
		"fn fib2 = fib1 + fib0", "fn fib3 = fib2 + fib1", "fn fib4 = fib3 + fib2" });
	ClearOutput();
	ExecuteCommand("printfns");
	std::string output = GetOutput();
	EXPECT_NE(output.find("fib0:0.00"), std::string::npos);
	EXPECT_NE(output.find("fib4:3.00"), std::string::npos);
}

TEST_F(CommandLineTest, AllOperations_BasicArithmetic)
{
	ExecuteCommands({ "let a = 10", "let b = 3", "fn add = a + b", "fn sub = a - b",
		"fn mul = a * b", "fn div = a / b" });
	EXPECT_DOUBLE_EQ(calc.GetValue("add").value(), 13.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sub").value(), 7.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("mul").value(), 30.0);
	EXPECT_NEAR(calc.GetValue("div").value(), 3.333333, 0.001);
}