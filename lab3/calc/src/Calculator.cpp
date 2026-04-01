#include "Calculator.h"
#include "Logger.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>

bool Calculator::IsValidIdentifier(const std::string& name)
{
	if (name.empty())
	{
		return false;
	}

	const char firstChar = name[0];
	if (!std::isalpha(firstChar) && firstChar != '_')
	{
		return false;
	}

	for (size_t i = 1; i < name.length(); ++i)
	{
		const char charOfName = name[i];
		if (!std::isalnum(charOfName) && charOfName != '_')
		{
			return false;
		}
	}

	return true;
}

bool Calculator::IsNameExists(const std::string& name) const
{
	if (name.empty())
	{
		return false;
	}

	return m_variables.contains(name)
		|| m_functions.contains(name);
}

bool Calculator::DeclareVariable(const std::string& name)
{
	if (!IsValidIdentifier(name))
	{
		Logger::Error("Invalid variable name: " + name);
		return false;
	}

	if (IsNameExists(name))
	{
		Logger::Error("Variable already exists: " + name);
		return false;
	}

	m_variables.emplace(name, Variable()); // создаем переменную внутри map без копий
	// лучше чем с копией m_variables[name] = Variable();
	Logger::Info("Variable name: " + name);
	return true;
}

bool Calculator::SetVariable(const std::string& name, const double value)
{
	if (!IsValidIdentifier(name))
	{
		Logger::Error("Invalid variable name: " + name);
		return false;
	}

	if (m_functions.contains(name))
	{
		Logger::Error("Name already exists as function: " + name);
		return false;
	}

	if (m_variables.contains(name))
	{
		m_variables[name].SetValue(value);
		Logger::Info("Variable updated: " + name + " = " + std::to_string(value));
		return true;
	}

	m_variables.emplace(name, Variable());
	m_variables[name].SetValue(value);
	Logger::Info("Variable created and set: " + name + " = " + std::to_string(value));
	return true;
}

bool Calculator::DeclareFunction(const std::string& name, const Function& func)
{
	if (!IsValidIdentifier(name))
	{
		Logger::Error("Invalid variable name: " + name);
		return false;
	}

	if (IsNameExists(name))
	{
		Logger::Error("Variable already exists: " + name);
		return false;
	}

	m_functions.emplace(name, func);
	Logger::Info("Function name: " + name);
	return true;
}

std::optional<double> Calculator::ComputeFunctionValue(const std::string& name, std::set<std::string>& visited)
{
	if (visited.contains(name))
	{
		Logger::Error("Cycle: " + name);
		return std::nan("");
	}
	visited.insert(name);

	const auto funcIt = m_functions.find(name);
	if (funcIt == m_functions.end())
	{
		visited.erase(name);
		return std::nullopt;
	}

	const Function& func = funcIt->second;

	const auto firstOpt = GetValue(func.GetOperand1());
	if (!firstOpt.has_value())
	{
		visited.erase(name);
		return std::nullopt;
	}

	if (!func.IsBinary())
	{
		visited.erase(name);
		return firstOpt;
	}

	const auto secondOpt = GetValue(func.GetOperand2());
	if (!secondOpt.has_value())
	{
		visited.erase(name);
		return std::nullopt;
	}

	const auto result = ApplyOperation(func.GetOperation(), firstOpt.value(), secondOpt.value(), name);

	visited.erase(name);
	return result;
}

std::optional<double> Calculator::GetOperandValue(const std::string& operandName)
{
	if (!IsValidIdentifier(operandName))
	{
		return std::nullopt;
	}

	auto varIt = m_variables.find(operandName);
	if (varIt != m_variables.end())
	{
		return varIt->second.GetValue();
	}

	auto funcIt = m_functions.find(operandName);
	if (funcIt != m_functions.end())
	{
		return std::nullopt;
	}

	return std::nullopt;
}

std::optional<double> Calculator::ApplyOperation(char op, double left, double right, const std::string& funcName)
{
	switch (op)
	{
	case '+':
		return left + right;
	case '-':
		return left - right;
	case '*':
		return left * right;
	case '/':
		if (right == 0.0)
		{
			Logger::Info("Division by zero in function: " + funcName);
			return std::nan("");
		}
		return left / right;
	default:
		Logger::Error("Unknown operation: " + std::string(1, op));
		return std::nan("");
	}
}

std::optional<double> Calculator::GetValue(const std::string& name)
{
	if (!IsValidIdentifier(name))
	{
		Logger::Error("Invalid variable name: " + name);
		return std::nullopt;
	}

	auto varIter = m_variables.find(name);
	if (varIter != m_variables.end())
	{
		return varIter->second.GetValue();
	}

	auto funcIter = m_functions.find(name);
	if (funcIter != m_functions.end())
	{
		std::set<std::string> visited;
		return ComputeFunctionValue(name, visited);
	}

	return std::nullopt;
}

void Calculator::PrintVariables()
{
	for (const auto& [name, var] : m_variables)
	{
		std::cout << name << ":";
		if (var.IsDefined())
		{
			std::cout << std::fixed << std::setprecision(2) << var.GetValue();
		}
		else
		{
			std::cout << "nan";
		}
		std::cout << std::endl;
	}
}

void Calculator::PrintFunctions()
{
	for (const auto& [name, func] : m_functions)
	{
		std::cout << name << ":";
		std::set<std::string> visited;
		auto value = ComputeFunctionValue(name, visited);
		if (value.has_value())
		{
			std::cout << std::fixed << std::setprecision(2) << value.value();
		}
		else
		{
			std::cout << "nan";
		}
		std::cout << std::endl;
	}
}