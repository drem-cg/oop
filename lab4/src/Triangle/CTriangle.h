#pragma once
#include "../Canvas/ICanvasDrawable.h"
#include "../Point/CPoint.h"
#include "Canvas/ICanvas.h"
#include "Canvas/ICanvasDrawable.h"
#include "ISolidShape.h"

#include <cstdint>

class CTriangle : public ISolidShape
	, public ICanvasDrawable
{
public:
	CTriangle(CPoint v1, CPoint v2, CPoint v3, uint32_t outlineColor, uint32_t fillColor);
	virtual ~CTriangle() override = default;

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;

	CPoint GetVertex1() const;
	CPoint GetVertex2() const;
	CPoint GetVertex3() const;

	void Draw(ICanvas& canvas) const;

private:
	CPoint m_v1, m_v2, m_v3;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
	double m_sideA, m_sideB, m_sideC;

	void CalculateSides();
	double Distance(CPoint p1, CPoint p2);
	double Distance(CPoint p1, CPoint p2) const;
};