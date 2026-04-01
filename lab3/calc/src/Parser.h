#pragma once

#include "Calculator.h"
#include <string>

class Parser
{
public:
	explicit Parser(Calculator& calculator);
	void Parse(const std::string& line);

private:
	Calculator& m_calculator;

	struct Assignment
	{
		std::string left;
		std::string right;
		bool isValid;
	};

	static Assignment ParseAssignment(const std::string& tail);
	static bool ValidateIdentifier(const std::string& identifier, const std::string& context);
	void HandleLetWithNumber(const std::string& leftName, double value) const;
	void HandleLetWithIdentifier(const std::string& leftName, const std::string& rightName) const;
	static void ErrorNameAlreadyExists(const std::string& name);

	struct OperatorInfo
	{
		char op;
		size_t position;
		bool found;
	};
	static OperatorInfo FindBinaryOperator(const std::string& expr);
	static std::optional<Function> CreateUnaryFunction(const std::string& operand);
	static std::optional<Function> CreateBinaryFunction(const std::string& expr, char op, size_t opPos) ;
	bool DeclareFunctionOrReport(const std::string& name, const Function& func) const;

	void ParseVarCommand(const std::string& tail) const;
	void ParseLetCommand(const std::string& tail);
	void ParseFnCommand(const std::string& tail) const;
	void ParsePrintCommand(const std::string& tail) const;
	static void ParseNumber(const std::string& str, double& outValue, bool& outSuccess);
	static std::string Trim(const std::string& str);
};