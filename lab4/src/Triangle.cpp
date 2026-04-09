#include "Triangle.h"
#include "Logger.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

constexpr double kMinArea = 0.0;

Triangle::Triangle(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const uint32_t strokeColor, const uint32_t fillColor)
	: ShapeBase(strokeColor, fillColor)
	, m_x1(x1)
	, m_y1(y1)
	, m_x2(x2)
	, m_y2(y2)
	, m_x3(x3)
	, m_y3(y3)
	, m_sideA(0.0)
	, m_sideB(0.0)
	, m_sideC(0.0)
{
	CalculateSides();

	const double area = Triangle::GetArea();
	if (area <= kMinArea)
	{
		LogError("Triangle is degenerate (collinear points)");
		throw std::invalid_argument("Triangle vertices must not be collinear");
	}
	LogDebug("Triangle created");
}

void Triangle::CalculateSides()
{
	m_sideA = Distance(m_x1, m_y1, m_x2, m_y2);
	m_sideB = Distance(m_x2, m_y2, m_x3, m_y3);
	m_sideC = Distance(m_x3, m_y3, m_x1, m_y1);
}

double Triangle::Distance(const double x1, const double y1, const double x2, const double y2)
{
	const double dx = x2 - x1;
	const double dy = y2 - y1;
	return std::sqrt(dx * dx + dy * dy);
}

double Triangle::GetArea() const
{
	return 0.5 * std::abs((m_x2 - m_x1) * (m_y3 - m_y1) - (m_x3 - m_x1) * (m_y2 - m_y1));
}

double Triangle::GetPerimeter() const { return m_sideA + m_sideB + m_sideC; }

std::string Triangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Triangle (v1=(" << m_x1 << "," << m_y1 << ")"
		<< ", v2=(" << m_x2 << "," << m_y2 << ")"
		<< ", v3=(" << m_x3 << "," << m_y3 << "))";
	return oss.str();
}

double Triangle::GetX1() const
{
	return m_x1;
}

double Triangle::GetY1() const
{
	return m_y1;
}

double Triangle::GetX2() const
{
	return m_x2;
}

double Triangle::GetY2() const
{
	return m_y2;
}

double Triangle::GetX3() const
{
	return m_x3;
}

double Triangle::GetY3() const
{
	return m_y3;
}