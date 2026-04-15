#pragma once

class ICanvas;

class ICanvasDrawable
{
public:
	virtual void Draw(ICanvas& canvas) const = 0;
	virtual ~ICanvasDrawable() = default;
};