#include "CRectangle.h"
#include "../Logger/Logger.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace
{
constexpr double kEpsilon = 1e-9;
}

CRectangle::CRectangle(const CPoint leftTop, const CPoint rightBottom, const uint32_t outlineColor, const uint32_t fillColor)
	: m_leftTop(leftTop)
	, m_rightBottom(rightBottom)
	, m_outlineColor(outlineColor)
	, m_fillColor(fillColor)
{
	m_width = std::abs(m_rightBottom.x - m_leftTop.x);
	m_height = std::abs(m_rightBottom.y - m_leftTop.y);

	if (m_width <= kEpsilon || m_height <= kEpsilon)
	{
		LogError("CRectangle dimensions must be positive");
		throw std::invalid_argument("CRectangle dimensions must be positive");
	}
	LogDebug("CRectangle created");
}

double CRectangle::GetArea() const
{
	return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
	return 2.0 * (m_width + m_height);
}

std::string CRectangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "Rectangle (leftTop=(" << m_leftTop.x << "," << m_leftTop.y << ")"
		<< ", width=" << m_width << ", height=" << m_height << ")";
	return oss.str();
}

uint32_t CRectangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CRectangle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CRectangle::GetLeftTop() const
{
	return m_leftTop;
}

CPoint CRectangle::GetRightBottom() const
{
	return m_rightBottom;
}

double CRectangle::GetWidth() const
{
	return m_width;
}

double CRectangle::GetHeight() const
{
	return m_height;
}

void CRectangle::Draw(ICanvas& canvas) const
{
	const double x1 = m_leftTop.x;
	const double y1 = m_leftTop.y;
	const double x2 = m_leftTop.x + m_width;
	const double y2 = m_leftTop.y + m_height;

	const std::vector polygon = {
		CPoint{ x1, y1 },
		CPoint{ x2, y1 },
		CPoint{ x2, y2 },
		CPoint{ x1, y2 }
	};
	canvas.FillPolygon(polygon, m_fillColor);

	canvas.DrawLine(CPoint{ x1, y1 }, CPoint{ x2, y1 }, m_outlineColor);
	canvas.DrawLine(CPoint{ x2, y1 }, CPoint{ x2, y2 }, m_outlineColor);
	canvas.DrawLine(CPoint{ x2, y2 }, CPoint{ x1, y2 }, m_outlineColor);
	canvas.DrawLine(CPoint{ x1, y2 }, CPoint{ x1, y1 }, m_outlineColor);
}