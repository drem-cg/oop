#pragma once
#include "../Canvas/ICanvas.h"
#include "../ISolidShape.h"
#include "../Point/CPoint.h"
#include "Canvas/ICanvasDrawable.h"

#include <cstdint>

class CRectangle : public ISolidShape
	, public ICanvasDrawable
{
public:
	CRectangle(CPoint leftTop, CPoint rightBottom, uint32_t outlineColor, uint32_t fillColor);

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	[[nodiscard]] uint32_t GetOutlineColor() const override;
	[[nodiscard]] uint32_t GetFillColor() const override;
	[[nodiscard]] CPoint GetLeftTop() const;
	[[nodiscard]] CPoint GetRightBottom() const;
	[[nodiscard]] double GetWidth() const;
	[[nodiscard]] double GetHeight() const;

	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_leftTop;
	CPoint m_rightBottom;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
	double m_width;
	double m_height;
};