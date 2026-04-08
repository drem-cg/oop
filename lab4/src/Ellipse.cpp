#include "Ellipse.h"
#include "Logger.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

constexpr double kPi = 3.14159265358979323846;
constexpr double kMinRadius = 1e-9;

Ellipse::Ellipse(const double x, const double y, const double rx, const double ry, const uint32_t strokeColor, const uint32_t fillColor)
	: ShapeBase(strokeColor, fillColor)
	, m_x(x)
	, m_y(y)
	, m_rx(rx)
	, m_ry(ry)
{
	if (m_rx <= kMinRadius || m_ry <= kMinRadius)
	{
		LogError("Ellipse radii must be positive");
		throw std::invalid_argument("Ellipse radii must be positive");
	}
	LogDebug("Ellipse created");
}

double Ellipse::GetArea() const { return kPi * m_rx * m_ry; }

double Ellipse::GetPerimeter() const
{
	const double h = (m_rx - m_ry) * (m_rx - m_ry) / ((m_rx + m_ry) * (m_rx + m_ry));
	return kPi * (m_rx + m_ry) * (1.0 + 3.0 * h / (10.0 + std::sqrt(4.0 - 3.0 * h)));
}

std::string Ellipse::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Ellipse (x=" << m_x << ", y=" << m_y
		<< ", rx=" << m_rx << ", ry=" << m_ry << ")";
	return oss.str();
}

void Ellipse::Draw(ICanvas& canvas) const
{
	canvas.SetStrokeColor(m_strokeColor);
	canvas.SetFillColor(m_fillColor);
	canvas.DrawEllipse(m_x, m_y, m_rx, m_ry);
}

double Ellipse::GetX() const
{
	return m_x;
}

double Ellipse::GetY() const
{
	return m_y;
}

double Ellipse::GetRx() const
{
	return m_rx;
}

double Ellipse::GetRy() const
{
	return m_ry;
}