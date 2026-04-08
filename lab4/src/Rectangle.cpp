#include "Rectangle.h"
#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

constexpr double kMinDimension = 1e-9;

Rectangle::Rectangle(const double x, const double y, const double width, const double height, const uint32_t strokeColor, const uint32_t fillColor)
	: ShapeBase(strokeColor, fillColor)
	, m_x(x)
	, m_y(y)
	, m_width(width)
	, m_height(height)
{
	if (m_width <= kMinDimension || m_height <= kMinDimension)
	{
		//LogError("Rectangle width and height must be positive");
		throw std::invalid_argument("Rectangle dimensions must be positive");
	}
	//LogDebug("Rectangle created");
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

void Rectangle::Draw(ICanvas& canvas) const
{
	canvas.SetStrokeColor(m_strokeColor);
	canvas.SetFillColor(m_fillColor);
	const double x2 = m_x + m_width;
	const double y2 = m_y + m_height;
	canvas.DrawLine(m_x, m_y, x2, m_y);
	canvas.DrawLine(x2, m_y, x2, y2);
	canvas.DrawLine(x2, y2, m_x, y2);
	canvas.DrawLine(m_x, y2, m_x, m_y);
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