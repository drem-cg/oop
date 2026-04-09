#pragma once
#include "ShapeBase.h"

class Triangle : public ShapeBase
{
public:
	Triangle(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t strokeColor, uint32_t fillColor);

	[[nodiscard]] double GetX1() const;
	[[nodiscard]] double GetY1() const;
	[[nodiscard]] double GetX2() const;
	[[nodiscard]] double GetY2() const;
	[[nodiscard]] double GetX3() const;
	[[nodiscard]] double GetY3() const;
	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;

private:
	double m_x1, m_y1;
	double m_x2, m_y2;
	double m_x3, m_y3;
	double m_sideA, m_sideB, m_sideC;

	void CalculateSides();
	static double Distance(double x1, double y1, double x2, double y2);
};