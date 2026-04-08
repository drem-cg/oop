#pragma once

#include <cstdint>

class ICanvas
{
public:
	virtual void SetStrokeColor(uint32_t color) = 0;
	virtual void SetFillColor(uint32_t color) = 0;
	virtual void DrawLine(double x1, double y1, double x2, double y2) = 0;
	virtual void DrawCircle(double cx, double cy, double r) = 0;
	virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
	virtual ~ICanvas() = default;
};