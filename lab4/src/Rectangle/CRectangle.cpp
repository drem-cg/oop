#include "CRectangle.h"
#include "../Logger/Logger.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

CRectangle::CRectangle(const CPoint leftTop, const CPoint rightBottom, const uint32_t outlineColor, const uint32_t fillColor)
	: m_leftTop(leftTop)
	, m_rightBottom(rightBottom)
	, m_outlineColor(outlineColor)
	, m_fillColor(fillColor)
{
	m_width = std::abs(m_rightBottom.x - m_leftTop.x);
	m_height = std::abs(m_rightBottom.y - m_leftTop.y);

	// TODO: не забыть вынести
	if (m_width <= 1e-9 || m_height <= 1e-9)
	{
		LogError("CRectangle dimensions must be positive");
		throw std::invalid_argument("CRectangle dimensions must be positive");
	}
	LogDebug("CRectangle created");
}

double CRectangle::GetArea() const { return m_width * m_height; }
double CRectangle::GetPerimeter() const { return 2.0 * (m_width + m_height); }

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
	const std::vector polygon = {
		m_leftTop,
		CPoint{ m_leftTop.x + m_width, m_leftTop.y },
		m_rightBottom,
		CPoint{ m_leftTop.x, m_rightBottom.y }
	};
	canvas.FillPolygon(polygon, m_fillColor);
	canvas.DrawLine(m_leftTop, CPoint{ m_leftTop.x + m_width, m_leftTop.y }, m_outlineColor);
	canvas.DrawLine(CPoint{ m_leftTop.x + m_width, m_leftTop.y }, m_rightBottom, m_outlineColor);
	canvas.DrawLine(m_rightBottom, CPoint{ m_leftTop.x, m_rightBottom.y }, m_outlineColor);
	canvas.DrawLine(CPoint{ m_leftTop.x, m_rightBottom.y }, m_leftTop, m_outlineColor);
}