#pragma once

#include <string>

class Function
{
public:
	// Для fn A = B + C
	Function(const std::string& operand1, const std::string& operand2, char operation);

	// Для fn A = B
	explicit Function(const std::string& operand1);

	std::string GetOperand1() const;
	std::string GetOperand2() const;
	char GetOperation() const;
	bool IsBinary() const;

private:
	std::string m_operand1;
	std::string m_operand2;
	char m_operation;
	bool m_isBinary;
};