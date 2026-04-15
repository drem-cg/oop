#include "CCircle.h"
#include "../Canvas/ICanvas.h"
#include "../Logger/Logger.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr double kPi = 3.14;
constexpr double kMinRadius = 0.0;
} // namespace

CCircle::CCircle(const CPoint center, const double radius, const uint32_t outlineColor, const uint32_t fillColor)
	: m_center(center)
	, m_radius(radius)
	, m_outlineColor(outlineColor)
	, m_fillColor(fillColor)
{
	if (m_radius <= kMinRadius)
	{
		LogError("CCircle radius must be positive");
		throw std::invalid_argument("CCircle radius must be positive");
	}
	LogDebug("CCircle created");
}

double CCircle::GetArea() const
{
	return kPi * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
	return 2.0 * kPi * m_radius;
}

std::string CCircle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Circle (center=(" << m_center.x << "," << m_center.y << "), r=" << m_radius << ")";
	return oss.str();
}

uint32_t CCircle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CCircle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CCircle::GetCenter() const
{
	return m_center;
}

double CCircle::GetRadius() const
{
	return m_radius;
}

void CCircle::Draw(ICanvas& canvas) const
{
	canvas.FillCircle(m_center, m_radius, m_fillColor);
	canvas.DrawCircle(m_center, m_radius, m_outlineColor);
}