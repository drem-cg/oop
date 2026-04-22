#pragma once
#include "IShape.h"

// TODO: вынести сюда m_color ..., в  ISolid вынести GetCOlor()

class ISolidShape : public IShape
{
public:
	[[nodiscard]] virtual uint32_t GetFillColor() const = 0;
	~ISolidShape() override = default;
};