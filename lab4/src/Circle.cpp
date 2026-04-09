#include "Circle.h"
#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

constexpr double kPi = 3.14;
constexpr double kMinRadius = 0.0;

Circle::Circle(const double x, const double y, const double radius, const uint32_t strokeColor, const uint32_t fillColor)
	: ShapeBase(strokeColor, fillColor)
	, m_x(x)
	, m_y(y)
	, m_radius(radius)
{
	if (m_radius <= kMinRadius)
	{
		LogError("Circle radius must be positive");
		throw std::invalid_argument("Circle radius must be positive");
	}
	LogDebug("Circle created");
}

double Circle::GetArea() const
{
	return kPi * m_radius * m_radius;
}

double Circle::GetPerimeter() const
{
	return 2.0 * kPi * m_radius;
}

std::string Circle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Circle (x=" << m_x << ", y=" << m_y
		<< ", r=" << m_radius << ")";
	return oss.str();
}

double Circle::GetX() const
{
	return m_x;
}

double Circle::GetY() const
{
	return m_y;
}

double Circle::GetRadius() const
{
	return m_radius;
}