#include "Parser.h"
#include "Logger.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Parser::Parser(Calculator& calculator)
	: m_calculator(calculator)
{
	RegisterCommands();
}

void Parser::RegisterCommands()
{
	m_commands = {
		{ "var", [this](const std::string& tail) { ParseVarCommand(tail); } },
		{ "let", [this](const std::string& tail) { ParseLetCommand(tail); } },
		{ "fn", [this](const std::string& tail) { ParseFnCommand(tail); } },
		{ "print", [this](const std::string& tail) { ParsePrintCommand(tail); } },
		{ "printvars", [this](const std::string&) { m_calculator.PrintVariables(); } },
		{ "printfns", [this](const std::string&) { m_calculator.PrintFunctions(); } },
	};
}

std::string Parser::Trim(const std::string& str)
{
	const size_t start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
	{
		return "";
	}
	const size_t end = str.find_last_not_of(" \t");
	if (end == std::string::npos)
	{
		return "";
	}
	return str.substr(start, end - start + 1);
}

void Parser::ParsePrintCommand(const std::string& tail) const
{
	const std::string trimmedName = Trim(tail);

	if (trimmedName.empty())
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("Print command: empty identifier");
		return;
	}

	if (!Calculator::IsValidIdentifier(trimmedName))
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("Print command: invalid identifier: " + trimmedName);
		return;
	}

	auto value = m_calculator.GetValue(trimmedName);

	if (value.has_value())
	{
		std::cout << std::fixed << std::setprecision(2) << value.value() << std::endl;
	}
	else
	{
		std::cout << "nan" << std::endl;
	}
}

void Parser::ParseNumber(const std::string& str, double& outValue, bool& outSuccess)
{
	outSuccess = false;
	outValue = 0.0;

	std::string trimmedStr = Trim(str);
	if (trimmedStr.empty())
	{
		return;
	}

	try
	{
		size_t position;
		outValue = std::stod(trimmedStr, &position);
		if (position == trimmedStr.length())
		{
			outSuccess = true;
		}
	}
	catch (...)
	{
		Logger::Error("Parse number error: " + trimmedStr);
		outSuccess = false;
	}
}

void Parser::Parse(const std::string& line) const
{
	std::istringstream iss(line);
	std::string command;
	iss >> command;

	std::string tail;
	std::getline(iss, tail);

	auto it = m_commands.find(command);
	if (it != m_commands.end())
	{
		it->second(tail);
	}
	else
	{
		std::cout << "Unknown command" << std::endl;
		Logger::Error("Unknown command: " + command);
	}
}

void Parser::ParseVarCommand(const std::string& tail) const
{
	const std::string trimmedName = Trim(tail);

	if (trimmedName.empty())
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("VAR parse error: " + tail);
		return;
	}

	if (!Calculator::IsValidIdentifier(trimmedName))
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("VAR identifier should be a valid identifier: " + tail);
		return;
	}

	if (!m_calculator.DeclareVariable(trimmedName))
	{
		std::cout << "Name already exists" << std::endl;
		Logger::Error("VAR declare error: " + trimmedName);
	}
}

void Parser::ParseLetCommand(const std::string& tail) const
{
	const auto assignment = ParseAssignment(tail);
	if (!assignment.isValid)
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("LetCommand: invalid assignment format");
		return;
	}

	if (!ValidateIdentifier(assignment.left, "LetCommand"))
	{
		return;
	}

	double value;
	bool isNumber;
	ParseNumber(assignment.right, value, isNumber);

	if (isNumber)
	{
		HandleLetWithNumber(assignment.left, value);
	}
	else
	{
		HandleLetWithIdentifier(assignment.left, assignment.right);
	}
}

Parser::Assignment Parser::ParseAssignment(const std::string& tail)
{
	const size_t eqPos = tail.find('=');
	if (eqPos == std::string::npos)
	{
		return { "", "", false };
	}

	Assignment result;
	result.left = Trim(tail.substr(0, eqPos));
	result.right = Trim(tail.substr(eqPos + 1));
	result.isValid = !result.left.empty() && !result.right.empty();
	return result;
}

bool Parser::ValidateIdentifier(const std::string& identifier, const std::string& context)
{
	if (identifier.empty() || !Calculator::IsValidIdentifier(identifier))
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error(context + ": invalid identifier: " + identifier);
		return false;
	}
	return true;
}

void Parser::HandleLetWithNumber(const std::string& leftName, const double value) const
{
	if (!m_calculator.SetVariable(leftName, value))
	{
		ErrorNameAlreadyExists(leftName);
	}
}

void Parser::HandleLetWithIdentifier(const std::string& leftName, const std::string& rightName) const
{
	if (!ValidateIdentifier(rightName, "LetCommand"))
	{
		return;
	}

	const auto sourceValue = m_calculator.GetValue(rightName);
	if (!sourceValue.has_value())
	{
		std::cout << "Name does not exist" << std::endl;
		Logger::Error("LetCommand: source identifier does not exist: " + rightName);
		return;
	}

	if (!m_calculator.SetVariable(leftName, sourceValue.value()))
	{
		ErrorNameAlreadyExists(leftName);
	}
}

void Parser::ErrorNameAlreadyExists(const std::string& name)
{
	std::cout << "Name already exists" << std::endl;
	Logger::Error("LetCommand: name already exists: " + name);
}

void Parser::ParseFnCommand(const std::string& tail) const
{
	const auto assignment = ParseAssignment(tail);
	if (!assignment.isValid || !ValidateIdentifier(assignment.left, "Fn command"))
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("Fn command: invalid assignment format");
		return;
	}

	const auto opInfo = FindBinaryOperator(assignment.right);

	std::optional<Function> func;
	if (!opInfo.found)
	{
		func = CreateUnaryFunction(assignment.right);
	}
	else
	{
		func = CreateBinaryFunction(assignment.right, opInfo.op, opInfo.position);
	}

	if (!func.has_value())
	{
		std::cout << "Invalid usage" << std::endl;
		Logger::Error("Fn command: invalid identifier in expression");
		return;
	}

	DeclareFunctionOrReport(assignment.left, func.value());
}

Parser::OperatorInfo Parser::FindBinaryOperator(const std::string& expr)
{
	for (size_t i = 0; i < expr.length(); ++i)
	{
		const char c = expr[i];
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			return { c, i, true };
		}
	}
	return { '\0', std::string::npos, false };
}

std::optional<Function> Parser::CreateUnaryFunction(const std::string& operand)
{
	const std::string trimmed = Trim(operand);
	if (!Calculator::IsValidIdentifier(trimmed))
	{
		return std::nullopt;
	}
	return Function(trimmed);
}

std::optional<Function> Parser::CreateBinaryFunction(
	const std::string& expr, const char op, const size_t opPos)
{
	const std::string op1 = Trim(expr.substr(0, opPos));
	const std::string op2 = Trim(expr.substr(opPos + 1));

	if (!Calculator::IsValidIdentifier(op1) || !Calculator::IsValidIdentifier(op2))
	{
		return std::nullopt;
	}
	return Function(op1, op2, op);
}

bool Parser::DeclareFunctionOrReport(const std::string& name, const Function& func) const
{
	if (!m_calculator.DeclareFunction(name, func))
	{
		std::cout << "Name already exists" << std::endl;
		Logger::Error("Fn command: name already exists: " + name);
		return false;
	}
	return true;
}