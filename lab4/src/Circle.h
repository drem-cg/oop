#pragma once
#include "ShapeBase.h"

class Circle : public ShapeBase
{
public:
	Circle(double x, double y, double radius, uint32_t strokeColor, uint32_t fillColor);
	~Circle() override = default;

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	void Draw(ICanvas& canvas) const override;

	[[nodiscard]] double GetX() const;
	[[nodiscard]] double GetY() const;
	[[nodiscard]] double GetRadius() const;

private:
	double m_x;
	double m_y;
	double m_radius;
};