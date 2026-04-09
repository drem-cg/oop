#include "Rectangle.h"
#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

constexpr double kMinDimension = 0.0;

Rectangle::Rectangle(const double x, const double y, const double width, const double height, const uint32_t strokeColor, const uint32_t fillColor)
	: ShapeBase(strokeColor, fillColor)
	, m_x(x)
	, m_y(y)
	, m_width(width)
	, m_height(height)
{
	if (m_width <= kMinDimension || m_height <= kMinDimension)
	{
		throw std::invalid_argument("Rectangle dimensions must be positive");
	}
}

double Rectangle::GetArea() const { return m_width * m_height; }
double Rectangle::GetPerimeter() const { return 2.0 * (m_width + m_height); }

std::string Rectangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Rectangle (x=" << m_x << ", y=" << m_y
		<< ", w=" << m_width << ", h=" << m_height << ")";
	return oss.str();
}

double Rectangle::GetX() const
{
	return m_x;
}

double Rectangle::GetY() const
{
	return m_y;
}

double Rectangle::GetWidth() const
{
	return m_width;
}

double Rectangle::GetHeight() const
{
	return m_height;
}