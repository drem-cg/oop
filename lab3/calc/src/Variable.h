#pragma once

#include <string>

class Variable
{
public:
	Variable();

	double GetValue() const;
	void SetValue(double value);
	bool IsDefined() const;

private:
	double m_value;
};