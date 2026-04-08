#pragma once
#include "ShapeBase.h"

class Rectangle : public ShapeBase
{
public:
	Rectangle(double x, double y, double width, double height, uint32_t strokeColor, uint32_t fillColor);
	~Rectangle() override = default;

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	void Draw(ICanvas& canvas) const override;

	[[nodiscard]] double GetX() const;
	[[nodiscard]] double GetY() const;
	[[nodiscard]] double GetWidth() const;
	[[nodiscard]] double GetHeight() const;

private:
	double m_x;
	double m_y;
	double m_width;
	double m_height;
};