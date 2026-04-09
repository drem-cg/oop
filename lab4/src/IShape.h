#pragma once

#include <cstdint>
#include <string>

class IShape
{
public:
	virtual ~IShape() = default;
	[[nodiscard]] virtual double GetArea() const = 0;
	[[nodiscard]] virtual double GetPerimeter() const = 0;
	[[nodiscard]] virtual uint32_t GetStrokeColor() const = 0;
	[[nodiscard]] virtual uint32_t GetFillColor() const = 0;
	[[nodiscard]] virtual std::string ToString() const = 0;
};