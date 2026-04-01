#include "Function.h"
#include "Logger.h"

Function::Function(const std::string& operand1, const std::string& operand2, char operation)
	: m_operand1(operand1)
	, m_operand2(operand2)
	, m_operation(operation)
	, m_isBinary(true)
{
	Logger::Info("Binary function created: " + operand1 + " " + operation + " " + operand2);
}

Function::Function(const std::string& operand1)
	: m_operand1(operand1)
	, m_operand2("")
	, m_operation('\0')
	, m_isBinary(false)
{
	Logger::Info("Unary function created: " + operand1);
}

std::string Function::GetOperand1() const
{
	return m_operand1;
}

std::string Function::GetOperand2() const
{
	return m_operand2;
}

char Function::GetOperation() const
{
	return m_operation;
}

bool Function::IsBinary() const
{
	return m_isBinary;
}