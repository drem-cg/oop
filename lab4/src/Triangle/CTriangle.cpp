#include "CTriangle.h"
#include "../Logger/Logger.h"
#include "Canvas/ICanvas.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

constexpr double kMinArea = 1e-9;

CTriangle::CTriangle(const CPoint v1, const CPoint v2, const CPoint v3, const uint32_t outlineColor, const uint32_t fillColor)
	: m_v1(v1)
	, m_v2(v2)
	, m_v3(v3)
	, m_outlineColor(outlineColor)
	, m_fillColor(fillColor)
	, m_sideA(0.0)
	, m_sideB(0.0)
	, m_sideC(0.0)
{
	CalculateSides();
	if (CTriangle::GetArea() <= kMinArea)
	{
		LogError("CTriangle is degenerate");
		throw std::invalid_argument("CTriangle vertices must not be collinear");
	}
	LogDebug("CTriangle created");
}

void CTriangle::CalculateSides()
{
	m_sideA = Distance(m_v1, m_v2);
	m_sideB = Distance(m_v2, m_v3);
	m_sideC = Distance(m_v3, m_v1);
}

double CTriangle::Distance(const CPoint p1, const CPoint p2)
{
	const double dx = p2.x - p1.x;
	const double dy = p2.y - p1.y;
	return std::sqrt(dx * dx + dy * dy);
}

double CTriangle::GetArea() const
{
	return 0.5 * std::abs((m_v2.x - m_v1.x) * (m_v3.y - m_v1.y) - (m_v3.x - m_v1.x) * (m_v2.y - m_v1.y));
}

double CTriangle::GetPerimeter() const { return m_sideA + m_sideB + m_sideC; }

std::string CTriangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Triangle (v1=(" << m_v1.x << "," << m_v1.y << ")"
		<< ", v2=(" << m_v2.x << "," << m_v2.y << ")"
		<< ", v3=(" << m_v3.x << "," << m_v3.y << "))";
	return oss.str();
}

uint32_t CTriangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CTriangle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CTriangle::GetVertex1() const
{
	return m_v1;
}

CPoint CTriangle::GetVertex2() const
{
	return m_v2;
}

CPoint CTriangle::GetVertex3() const
{
	return m_v3;
}

void CTriangle::Draw(ICanvas& canvas) const
{
	const std::vector<CPoint> polygon = { m_v1, m_v2, m_v3 };
	canvas.FillPolygon(polygon, m_fillColor);
	canvas.DrawLine(m_v1, m_v2, m_outlineColor);
	canvas.DrawLine(m_v2, m_v3, m_outlineColor);
	canvas.DrawLine(m_v3, m_v1, m_outlineColor);
}