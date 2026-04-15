#include "CLineSegment.h"
#include "../Logger/Logger.h"
#include <cmath>
#include <iomanip>
#include <sstream>

CLineSegment::CLineSegment(const CPoint start, const CPoint end, const uint32_t outlineColor)
	: m_start(start)
	, m_end(end)
	, m_outlineColor(outlineColor)
{
	LogDebug("CLineSegment created");
}

double CLineSegment::GetArea() const
{
	return 0.0;
}

double CLineSegment::GetPerimeter() const
{
	const double dx = m_end.x - m_start.x;
	const double dy = m_end.y - m_start.y;
	return std::sqrt(dx * dx + dy * dy);
}

std::string CLineSegment::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2)
		<< "LineSegment (start=(" << m_start.x << "," << m_start.y << ")"
		<< ", end=(" << m_end.x << "," << m_end.y << "))";
	return oss.str();
}

uint32_t CLineSegment::GetOutlineColor() const
{
	return m_outlineColor;
}

CPoint CLineSegment::GetStartPoint() const
{
	return m_start;
}

CPoint CLineSegment::GetEndPoint() const
{
	return m_end;
}