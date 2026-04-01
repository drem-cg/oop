#include "../src/Calculator.h"
#include "../src/Parser.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

class ParserTest : public ::testing::Test
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

TEST_F(ParserTest, Var_ValidAndInvalid)
{
	ExecuteCommand("var x");
	EXPECT_TRUE(calc.GetValue("x").has_value());

	ClearOutput();
	ExecuteCommand("var ");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");

	ClearOutput();
	ExecuteCommand("var 123invalid");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");

	ClearOutput();
	ExecuteCommand("var x");
	EXPECT_EQ(GetOutput(), "Name already exists\n");
}

TEST_F(ParserTest, Let_NumberAndVariable)
{
	ExecuteCommand("let x = 42.5");
	ASSERT_TRUE(calc.GetValue("x").has_value());
	EXPECT_DOUBLE_EQ(calc.GetValue("x").value(), 42.5);

	ExecuteCommand("let y = x");
	ASSERT_TRUE(calc.GetValue("y").has_value());
	EXPECT_DOUBLE_EQ(calc.GetValue("y").value(), 42.5);

	ClearOutput();
	ExecuteCommand("let z = nonexistent");
	EXPECT_EQ(GetOutput(), "Name does not exist\n");

	ClearOutput();
	ExecuteCommand("let x 10");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(ParserTest, Let_EmptyOperands)
{
	ClearOutput();
	ExecuteCommand("let = 10");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");

	ClearOutput();
	ExecuteCommand("let x =");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(ParserTest, Fn_UnaryAndBinary)
{
	ExecuteCommands({ "var x", "var y", "fn f = x", "fn sum = x + y" });
	ExecuteCommands({ "let x = 5", "let y = 3" });

	EXPECT_DOUBLE_EQ(calc.GetValue("f").value(), 5.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sum").value(), 8.0);

	ClearOutput();
	ExecuteCommand("fn f x");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(ParserTest, Fn_DuplicateName)
{
	ExecuteCommands({ "var x", "fn f = x" });
	ClearOutput();
	ExecuteCommand("fn f = x");
	EXPECT_EQ(GetOutput(), "Name already exists\n");
}

TEST_F(ParserTest, Print_ValueAndErrors)
{
	ExecuteCommand("let x = 42");
	ExecuteCommand("print x");
	EXPECT_EQ(GetOutput(), "42.00\n");

	ClearOutput();
	ExecuteCommand("var y");
	ExecuteCommand("print y");
	EXPECT_EQ(GetOutput(), "nan\n");

	ClearOutput();
	ExecuteCommand("print nonexistent");
	EXPECT_EQ(GetOutput(), "nan\n");

	ClearOutput();
	ExecuteCommand("print ");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}

TEST_F(ParserTest, PrintVars_Printfns)
{
	ExecuteCommands({ "let x = 10", "let y = 20" });
	ClearOutput();
	ExecuteCommand("printvars");
	std::string output = GetOutput();
	EXPECT_NE(output.find("x:10.00"), std::string::npos);
	EXPECT_NE(output.find("y:20.00"), std::string::npos);

	ClearOutput();
	ExecuteCommand("var z");
	ExecuteCommand("printvars");
	output = GetOutput();
	EXPECT_NE(output.find("z:nan"), std::string::npos);

	ExecuteCommands({ "var a", "var b", "fn f = a + b", "let a = 5", "let b = 3" });
	ClearOutput();
	ExecuteCommand("printfns");
	output = GetOutput();
	EXPECT_NE(output.find("f:8.00"), std::string::npos);
}

TEST_F(ParserTest, UnknownCommand)
{
	ClearOutput();
	ExecuteCommand("unknowncommand");
	EXPECT_EQ(GetOutput(), "Unknown command\n");
}

TEST_F(ParserTest, EdgeCase_SpacesAndTabs)
{
	ExecuteCommand("let    x    =    42");
	ASSERT_TRUE(calc.GetValue("x").has_value());
	EXPECT_DOUBLE_EQ(calc.GetValue("x").value(), 42.0);

	ExecuteCommand("var \t y \t ");
	EXPECT_TRUE(calc.GetValue("y").has_value());
}

TEST_F(ParserTest, EdgeCase_InvalidIdentifiers)
{
	ClearOutput();
	ExecuteCommand("var 123start");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");

	ClearOutput();
	ExecuteCommand("var my-var");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");

	ClearOutput();
	ExecuteCommand("var my.var");
	EXPECT_EQ(GetOutput(), "Invalid usage\n");
}