#pragma once
#include "../Canvas/ICanvasDrawable.h"
#include "../Point/CPoint.h"
#include "ISolidShape.h"

#include <cstdint>

class CTriangle : public ISolidShape
	, public ICanvasDrawable
{
public:
	CTriangle(CPoint v1, CPoint v2, CPoint v3, uint32_t outlineColor, uint32_t fillColor);

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	[[nodiscard]] uint32_t GetOutlineColor() const override;
	[[nodiscard]] uint32_t GetFillColor() const override;

	[[nodiscard]] CPoint GetVertex1() const;
	[[nodiscard]] CPoint GetVertex2() const;
	[[nodiscard]] CPoint GetVertex3() const;

	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_v1, m_v2, m_v3;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
	double m_sideA, m_sideB, m_sideC;
};