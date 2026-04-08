#pragma once

#include "ICanvasDrawable.h"
#include "IShape.h"

#include <cstdint>

class ShapeBase : public IShape
	, public ICanvasDrawable
{
public:
	ShapeBase(uint32_t strokeColor, uint32_t fillColor);
	~ShapeBase() override = default;

	[[nodiscard]] uint32_t GetStrokeColor() const override;
	[[nodiscard]] uint32_t GetFillColor() const override;

protected:
	uint32_t m_strokeColor;
	uint32_t m_fillColor;
};