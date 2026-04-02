#pragma once

#include "Function.h"
#include "Variable.h"
#include <map>
#include <optional>
#include <set>
#include <string>

class Calculator
{
public:
	bool DeclareVariable(const std::string& name);
	bool SetVariable(const std::string& name, double value);
	bool DeclareFunction(const std::string& name, const Function& func);
	std::optional<double> GetValue(const std::string& name);
	void PrintVariables();
	void PrintFunctions();

	static bool IsValidIdentifier(const std::string& name);

private:
	std::map<std::string, Variable> m_variables;
	std::map<std::string, Function> m_functions;

	static std::optional<double> ApplyOperation(char op, double left, double right, const std::string& funcName);
	std::optional<double> GetOperandValue(const std::string& operandName);
	std::optional<double> ComputeUnaryFunction(const Function& func);
	std::optional<double> ComputeBinaryFunction(const Function& func);
	std::optional<double> ComputeFunctionValue(const std::string& name);
	bool IsNameExists(const std::string& name) const;
};