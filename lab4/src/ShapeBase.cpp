#include "ShapeBase.h"

ShapeBase::ShapeBase(const uint32_t strokeColor, const uint32_t fillColor)
	: m_strokeColor(strokeColor)
	, m_fillColor(fillColor)
{
}

uint32_t ShapeBase::GetStrokeColor() const
{
	return m_strokeColor;
}

uint32_t ShapeBase::GetFillColor() const
{
	return m_fillColor;
}