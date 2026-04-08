#pragma once
#include "ShapeBase.h"

class Ellipse : public ShapeBase
{
public:
	Ellipse(double x, double y, double rx, double ry, uint32_t strokeColor, uint32_t fillColor);
	~Ellipse() override = default;

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	void Draw(ICanvas& canvas) const override;

	[[nodiscard]] double GetX() const;
	[[nodiscard]] double GetY() const;
	[[nodiscard]] double GetRx() const;
	[[nodiscard]] double GetRy() const;

private:
	double m_x;
	double m_y;
	double m_rx;
	double m_ry;
};