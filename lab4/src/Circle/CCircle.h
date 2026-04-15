#pragma once
#include "../Canvas/ICanvasDrawable.h"
#include "../ISolidShape.h"
#include "../Point/CPoint.h"
#include <cstdint>

class CCircle : public ISolidShape
{
public:
	CCircle(CPoint center, double radius, uint32_t outlineColor, uint32_t fillColor);
	~CCircle() override = default;

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	[[nodiscard]] uint32_t GetOutlineColor() const override;
	[[nodiscard]] uint32_t GetFillColor() const override;
	[[nodiscard]] CPoint GetCenter() const;
	[[nodiscard]] double GetRadius() const;

	void Draw(ICanvas& canvas) const;

private:
	CPoint m_center;
	double m_radius;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
};