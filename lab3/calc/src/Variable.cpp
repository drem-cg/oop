#include "Variable.h"
#include "Logger.h"
#include <cmath>

Variable::Variable()
	: m_value(std::nan(""))
{
	Logger::Info("Variable created (undefined)");
}

double Variable::GetValue() const
{
	return m_value;
}

void Variable::SetValue(double value)
{
	m_value = value;
	Logger::Info("Variable set to: " + std::to_string(value));
}

bool Variable::IsDefined() const
{
	return !std::isnan(m_value);
}