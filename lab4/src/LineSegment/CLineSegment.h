#pragma once
#include "../IShape.h"
#include "../Point/CPoint.h"
#include <cstdint>

class CLineSegment : public IShape
{
public:
	CLineSegment(CPoint start, CPoint end, uint32_t outlineColor);
	~CLineSegment() override = default;

	[[nodiscard]] double GetArea() const override;
	[[nodiscard]] double GetPerimeter() const override;
	[[nodiscard]] std::string ToString() const override;
	[[nodiscard]] uint32_t GetOutlineColor() const override;
	[[nodiscard]] CPoint GetStartPoint() const;
	[[nodiscard]] CPoint GetEndPoint() const;

private:
	CPoint m_start;
	CPoint m_end;
	uint32_t m_outlineColor;
};